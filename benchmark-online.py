#! /usr/bin/python
import subprocess
import time
import sys

def runTests(num_clients=2, num_trials=5):

	time_chisq_2attr = []
	time_chisq_5attr = []
	time_chisq_12attr = []

	time_ttest_abalone = []
	time_pearson_abalone = []

	time_ttest_1000 = []
	time_pearson_1000 = []

	time_ttest_5000 = []
	time_pearson_5000 = []

	time_ttest_10000 = []
	time_pearson_10000 = []

	# 
	# CHI SQUARED RUNTIME
	#
	for i in range(num_trials):
		# chisq with two selected attributes
		time_start = time.time()
		subprocess.call(['./client.x', str(num_clients), '0', '--chisq', '2', '0', '1', '0', \
			'/home/sachaservanschreiber/star-spdz/HOSTS'])
		time_elapsed = (time.time() - time_start)
		time_chisq_2attr.append(time_elapsed)

		# chisq with 5 selected attributes
		time_start = time.time()
		subprocess.call(['./client.x', str(num_clients), '0', '--chisq', '5', '0', '1', '2', '3', '4', '0', \
			'/home/sachaservanschreiber/star-spdz/HOSTS'])
		time_elapsed = (time.time() - time_start)
		time_chisq_5attr.append(time_elapsed)

		# chisq with 12 selected attributes
		time_start = time.time()
		subprocess.call(['./client.x', str(num_clients), '0', '--chisq', '12', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '0', \
			'/home/sachaservanschreiber/star-spdz/HOSTS'])
		time_elapsed = (time.time() - time_start)
		time_chisq_12attr.append(time_elapsed)

	with open("Benchmark-Results-Chisq.txt", "w+") as text_file:
		text_file.write("===================================\n")
		text_file.write("chisq @ 2: " + str(time_chisq_2attr) + "\n")
		text_file.write("chisq @ 5: " + str(time_chisq_5attr) + "\n")
		text_file.write("chisq @ 12: " + str(time_chisq_12attr) + "\n")
		text_file.write("===================================\n")

	#
	#
	# Abalone dataset
	#
	#
	subprocess.call(['./upload-client.x',\
		'/home/sachaservanschreiber/star-spdz/datasets/abalone_continuous.txt', \
		str(num_clients),'0', '/home/sachaservanschreiber/star-spdz/HOSTS'])

	for i in range(num_trials):
		time_start = time.time()
		subprocess.call(['./client.x',str(num_clients), '0', '--ttest', '2', '0', '1', '0', \
			'/home/sachaservanschreiber/star-spdz/HOSTS'])
		time_elapsed = (time.time() - time_start)
		time_ttest_abalone.append(time_elapsed)


	for i in range(num_trials):
		time_start = time.time()
		subprocess.call(['./client.x', str(num_clients), '0', '--pearson', '2', '0', '1', '0', \
			'/home/sachaservanschreiber/star-spdz/HOSTS'])
		time_elapsed = (time.time() - time_start)
		time_pearson_abalone.append(time_elapsed)

	with open("Benchmark-Results-Abalone.txt", "w+") as text_file:
		text_file.write("===================================\n")
		text_file.write("t-test abalone: " + str(time_ttest_abalone) + "\n")
		text_file.write("===================================\n")
		text_file.write("===================================\n")
		text_file.write("pearson abalone: " + str(time_pearson_abalone) + "\n")
		text_file.write("===================================\n")


	#
	#
	# 1000 patient dataset
	#
	#
	subprocess.call(['./upload-client.x',\
		'/home/sachaservanschreiber/star-spdz/datasets/patient_continuous_1000.txt', \
		str(num_clients),'0', '/home/sachaservanschreiber/star-spdz/HOSTS'])

	for i in range(num_trials):
		time_start = time.time()
		subprocess.call(['./client.x', str(num_clients), '0', '--ttest', '2', '0', '1', '0', \
			'/home/sachaservanschreiber/star-spdz/HOSTS'])
		time_elapsed = (time.time() - time_start)
		time_ttest_1000.append(time_elapsed)

	for i in range(num_trials):
		time_start = time.time()
		subprocess.call(['./client.x', str(num_clients), '0', '--pearson', '2', '0', '1', '0', \
			'/home/sachaservanschreiber/star-spdz/HOSTS'])
		time_elapsed = (time.time() - time_start)
		time_pearson_1000.append(time_elapsed)

	with open("Benchmark-Results-1000.txt", "w+") as text_file:
		text_file.write("===================================\n")
		text_file.write("t-test 1,000: " + str(time_ttest_1000) + "\n")
		text_file.write("===================================\n")
		text_file.write("===================================\n")
		text_file.write("pearson 1,000: " + str(time_pearson_1000) + "\n")
		text_file.write("===================================\n")

	#
	#
	# 5000 patient dataset
	#
	#
	subprocess.call(['./upload-client.x',\
		'/home/sachaservanschreiber/star-spdz/datasets/patient_continuous_5000.txt', \
		str(num_clients),'0', '/home/sachaservanschreiber/star-spdz/HOSTS'])

	for i in range(num_trials):
		time_start = time.time()
		subprocess.call(['./client.x', str(num_clients), '0', '--ttest', '2', '0', '1', '0', \
			'/home/sachaservanschreiber/star-spdz/HOSTS'])
		time_elapsed = (time.time() - time_start)
		time_ttest_5000.append(time_elapsed)

	for i in range(num_trials):
		time_start = time.time()
		subprocess.call(['./client.x', str(num_clients), '0', '--pearson', '2', '0', '1', '0', \
			'/home/sachaservanschreiber/star-spdz/HOSTS'])
		time_elapsed = (time.time() - time_start)
		time_pearson_5000.append(time_elapsed)


	with open("Benchmark-Results-5000.txt", "w+") as text_file:
		text_file.write("===================================\n")
		text_file.write("t-test 5,000: " + str(time_ttest_5000) + "\n")
		text_file.write("===================================\n")
		text_file.write("===================================\n")
		text_file.write("pearson 5,000: " + str(time_pearson_5000) + "\n")
		text_file.write("===================================\n")

	#
	#
	# 5000 patient dataset
	#
	#
	subprocess.call(['./upload-client.x',\
		'/home/sachaservanschreiber/star-spdz/datasets/patient_continuous_10000.txt', \
		str(num_clients),'0', '/home/sachaservanschreiber/star-spdz/HOSTS'])

	for i in range(num_trials):
		time_start = time.time()
		subprocess.call(['./client.x', str(num_clients), '0', '--ttest', '2', '0', '1', '0', \
			'/home/sachaservanschreiber/star-spdz/HOSTS'])
		time_elapsed = (time.time() - time_start)
		time_ttest_10000.append(time_elapsed)

	for i in range(num_trials):
		time_start = time.time()
		subprocess.call(['./client.x', str(num_clients), '0', '--pearson', '2', '0', '1', '0', \
			'/home/sachaservanschreiber/star-spdz/HOSTS'])
		time_elapsed = (time.time() - time_start)
		time_pearson_10000.append(time_elapsed)

	with open("Benchmark-Results-10000.txt", "w+") as text_file:
		text_file.write("===================================\n")
		text_file.write("t-test 10,000: " + str(time_ttest_10000) + "\n")
		text_file.write("===================================\n")
		text_file.write("===================================\n")
		text_file.write("pearson 10,000: " + str(time_pearson_10000) + "\n")
		text_file.write("===================================\n")

if __name__ == "__main__":
    if (len(sys.argv) < 2 or int(sys.argv[1]) < 2):
        print("Usage: ./benchmark-online.py <num parties> <num trials>")
        sys.exit(1)

   
    num_parties = int(sys.argv[1])
    num_trials = int(sys.argv[2])

    runTests(num_parties, num_trials)
