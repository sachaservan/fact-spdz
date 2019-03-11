#! /usr/bin/python
import subprocess
import time

time_chisq_2attr = []
time_chisq_5attr = []
time_chisq_12attr = []

time_ttest_1000 = []
time_pearson_1000 = []
time_ftest_1000 = []

time_ttest_5000 = []
time_pearson_5000 = []
time_ftest_5000 = []

time_ttest_10000 = []
time_pearson_10000 = []
time_ftest_10000 = []

# 
# CHI SQUARED RUNTIME
#
for i in range(num_tests):
	# chisq with two selected attributes
	time_start = time.clock()
	subprocess.call(['./client.x', '2', '0', '--chisq', '2', '0', '1', '0', \
		'/home/sachaservanschreiber/star-spdz/HOSTS'])
	time_elapsed = (time.clock() - time_start)
	time_chisq_2attr.append(time_elapsed)

	# chisq with 5 selected attributes
	time_start = time.clock()
	subprocess.call(['./client.x', '2', '0', '--chisq', '5', '0', '1', '2', '3', '4', '0', \
		'/home/sachaservanschreiber/star-spdz/HOSTS'])
	time_elapsed = (time.clock() - time_start)
	time_chisq_5attr.append(time_elapsed)

	# chisq with 12 selected attributes
	time_start = time.clock()
	subprocess.call(['./client.x', '2', '0', '--chisq', '12', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '0', \
		'/home/sachaservanschreiber/star-spdz/HOSTS'])
	time_elapsed = (time.clock() - time_start)
	time_chisq_12attr.append(time_elapsed)

# 
# TTEST RUNTIME
#
# upload continuous dataset #5000
subprocess.call(['./upload-client.x',\
	'/home/sachaservanschreiber/star-spdz/datasets/patient_continuous_1000.txt', \
	'2','0', '/home/sachaservanschreiber/star-spdz/HOSTS'])
for i in range(num_tests):
	time_start = time.clock()
	subprocess.call(['./client.x', '2', '0', '--ttest', '2', '0', '1', '0', \
		'/home/sachaservanschreiber/star-spdz/HOSTS'])
	time_elapsed = (time.clock() - time_start)
	time_ttest_1000.append(time_elapsed)

for i in range(num_tests):
	time_start = time.clock()
	subprocess.call(['./client.x', '2', '0', '--pearson', '2', '0', '1', '0', \
		'/home/sachaservanschreiber/star-spdz/HOSTS'])
	time_elapsed = (time.clock() - time_start)
	time_pearson_1000.append(time_elapsed)

for i in range(num_tests):
	time_start = time.clock()
	subprocess.call(['./client.x', '2', '0', '--ftest', '2', '0', '1', '0', \
		'/home/sachaservanschreiber/star-spdz/HOSTS'])
	time_elapsed = (time.clock() - time_start)
	time_ftest_1000.append(time_elapsed)

print("Chisq 2: " + str(time_chisq_2attr))
print("Chisq 5: " + str(time_chisq_5attr))
print("Chisq 12: " + str(time_chisq_12attr))
print("Ttest 1000: " + str(time_ttest_1000))

exit(0)

# upload continuous dataset #5000
subprocess.call(['./upload-client.x',\
	'/home/sachaservanschreiber/star-spdz/datasets/patient_continuous_5000.txt', \
	'2','0', '/home/sachaservanschreiber/star-spdz/HOSTS'])

# TODO: perform tests

# upload continuous dataset #10000
subprocess.call(['./upload-client.x',\
	'/home/sachaservanschreiber/star-spdz/datasets/patient_continuous_10000.txt', \
	'2','0', '/home/sachaservanschreiber/star-spdz/HOSTS'])

# TODO: perform tests