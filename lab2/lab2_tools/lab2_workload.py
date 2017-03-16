#!/usr/bin/env python
# ---------------------------------------------------
# Multicore Programming (Labs 2 - 4)
# httperf Workload Generator
# Copyright (c) 2016 Dr. Christopher Mitchell
#
# Usage: lab2_workload.py [-n <number of requests>]
# ---------------------------------------------------
import os, sys
import argparse

from reqgen import ReqGen

DEFAULT_REQS = 10000

def main():
	# Parse command line arguments
	parser = argparse.ArgumentParser(description="Generates Lab 2 httperf test workloads")
	parser.add_argument('-n', required=False, type=int, default=DEFAULT_REQS, \
	                    help='Number of requests to generate')
	parser.add_argument('-z', '--zipfian', action='store_true', \
	                    help='Generate a Zipfian, not uniform, distribution of keys')
	args = parser.parse_args()

	# Generate workload
	req_gen = ReqGen()
	for i in xrange(args.n):
		sys.stdout.write(req_gen.genRequest(args.zipfian))

if __name__ == "__main__":
	main()
