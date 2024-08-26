/**
 * Reimplementation of https://github.com/acoustid/pg_acoustid/blob/main/acoustid_compare.c
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "compare.h"

const uint64_t M1 = 0x5555555555555555ULL;
const uint64_t M2 = 0x3333333333333333ULL;
const uint64_t M4 = 0x0f0f0f0f0f0f0f0fULL;
const uint64_t H01 = 0x0101010101010101ULL;

int popcount (uint64_t x) {
	x -= (x >> 1) & M1;
	x = (x & M2) + ((x >> 2) & M2);
	x = (x + (x >> 4)) & M4;
	return (x * H01) >> 56;
}

float match_fingerprints (signed int *a, int asize, signed int *b, int bsize, int maxoffset) {
	int i, topcount, topoffset, size, biterror, minsize, auniq = 0, buniq = 0;
	int numcounts = asize + bsize + 1;
	unsigned short *counts = calloc(numcounts, sizeof(unsigned short));
	uint8_t *seen;
	uint16_t *aoffsets, *boffsets;
	uint64_t *adata, *bdata;
	float score, diversity;

	aoffsets = calloc((MATCH_MASK + 1) * 2, sizeof(uint16_t));
	boffsets = aoffsets + MATCH_MASK + 1;
	seen = (uint8_t *)aoffsets;

	for (i = 0; i < asize; i++) {
		aoffsets[MATCH_STRIP(a[i])] = i;
	}

	for (i = 0; i < bsize; i++) {
		boffsets[MATCH_STRIP(b[i])] = i;
	}

	topcount = 0;
	topoffset = 0;
	for (i = 0; i < MATCH_MASK; i++) {
		if (aoffsets[i] && boffsets[i]) {
			int offset = aoffsets[i] - boffsets[i];
			if (maxoffset == 0 || (-maxoffset <= offset && offset <= maxoffset)) {
				offset += bsize;
				counts[offset]++;
				if (counts[offset] > topcount) {
					topcount = counts[offset];
					topoffset = offset;
				}
			}
		}
	}

	topoffset -= bsize;

	minsize = MIN(asize, bsize) & ~1;
	if (topoffset < 0) {
		b -= topoffset;
		bsize = MAX(0, bsize + topoffset);
	}
	else {
		a += topoffset;
		asize = MAX(0, asize - topoffset);
	}

	size = MIN(asize, bsize) / 2;
	if (!size || !minsize) {
		printf("acoustid_compare: empty matching subfingerprint\n");
		score = 0.0;
		goto exit;
	}

	memset(seen, 0, UNIQ_MASK);
	for (i = 0; i < asize; i++) {
		int key = UNIQ_STRIP(a[i]);
		if (!seen[key]) {
			auniq++;
			seen[key] = 1;
		}
	}

	memset(seen, 0, UNIQ_MASK);
	for (i = 0; i < bsize; i++) {
		int key = UNIQ_STRIP(b[i]);
		if (!seen[key]) {
			buniq++;
			seen[key] = 1;
		}
	}

	diversity = MIN(MIN(1.0, (float)(auniq + 10) / asize + 0.5),
	                MIN(1.0, (float)(buniq + 10) / bsize + 0.5));

	if (topcount < MAX(auniq, buniq) * 0.02) {
		printf("acoustid_compare: top offset score is below 2%% of the unique size\n");
		score = 0.0;
		goto exit;
	}

	adata = (uint64_t *)a;
	bdata = (uint64_t *)b;
	biterror = 0;
	for (i = 0; i < size; i++, adata++, bdata++) {
		biterror += popcount(*adata ^ *bdata);
	}
	score = (size * 2.0 / minsize) * (1.0 - 2.0 * (float)biterror / (64 * size));
	if (score < 0.0) {
		score = 0.0;
	}
	if (diversity < 1.0) {
		float newscore = pow(score, 8.0 - 7.0 * diversity);
		printf("acoustid_compare2: scaling score because of duplicate items, %f => %f\n", score, newscore);
		score = newscore;
	}
exit:
	free(aoffsets);
	free(counts);
	return score;
}
