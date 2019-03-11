#! /usr/bin/python
import subprocess
import time

num_trials = 5

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
for i in range(num_trials):
	# chisq with two selected attributes
	time_start = time.time()
	subprocess.call(['./client.x', '2', '0', '--chisq', '2', '0', '1', '0', \
		'/home/sachaservanschreiber/star-spdz/HOSTS'])
	time_elapsed = (time.time() - time_start)
	time_chisq_2attr.append(time_elapsed)

	# chisq with 5 selected attributes
	time_start = time.time()
	subprocess.call(['./client.x', '2', '0', '--chisq', '5', '0', '1', '2', '3', '4', '0', \
		'/home/sachaservanschreiber/star-spdz/HOSTS'])
	time_elapsed = (time.time() - time_start)
	time_chisq_5attr.append(time_elapsed)

	# chisq with 12 selected attributes
	time_start = time.time()
	subprocess.call(['./client.x', '2', '0', '--chisq', '12', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '0', \
		'/home/sachaservanschreiber/star-spdz/HOSTS'])
	time_elapsed = (time.time() - time_start)
	time_chisq_12attr.append(time_elapsed)

print("===================================")
print("chisq @ 2: " + str(time_chisq_2attr))
print("chisq @ 5: " + str(time_chisq_5attr))
print("chisq @ 12: " + str(time_chisq_12attr))
print("===================================")

# 
# patient_continuous_1000.txt runtime
#
# upload continuous dataset #5000
subprocess.call(['./upload-client.x',\
	'/home/sachaservanschreiber/star-spdz/datasets/patient_continuous_1000.txt', \
	'2','0', '/home/sachaservanschreiber/star-spdz/HOSTS'])

for i in range(num_trials):
	time_start = time.time()
	subprocess.call(['./client.x', '2', '0', '--ttest', '2', '0', '1', '0', \
		'/home/sachaservanschreiber/star-spdz/HOSTS'])
	time_elapsed = (time.time() - time_start)
	time_ttest_1000.append(time_elapsed)

print("===================================")
print("t-test 1,000: " + str(time_ttest_1000))
print("===================================")

for i in range(num_trials):
	time_start = time.time()
	subprocess.call(['./client.x', '2', '0', '--pearson', '2', '0', '1', '0', \
		'/home/sachaservanschreiber/star-spdz/HOSTS'])
	time_elapsed = (time.time() - time_start)
	time_pearson_1000.append(time_elapsed)

print("===================================")
print("pearson 1,000: " + str(time_pearson_1000))
print("===================================")

for i in range(num_trials):
	time_start = time.time()
	subprocess.call(['./client.x', '2', '0', '--ftest', '2', '0', '1', '0', \
		'/home/sachaservanschreiber/star-spdz/HOSTS'])
	time_elapsed = (time.time() - time_start)
	time_ftest_1000.append(time_elapsed)

print("===================================")
print("f-test 1,000: " + str(time_ftest_1000))
print("===================================")


# upload continuous dataset #5000
subprocess.call(['./upload-client.x',\
	'/home/sachaservanschreiber/star-spdz/datasets/patient_continuous_5000.txt', \
	'2','0', '/home/sachaservanschreiber/star-spdz/HOSTS'])

for i in range(num_trials):
	time_start = time.time()
	subprocess.call(['./client.x', '2', '0', '--ttest', '2', '0', '1', '0', \
		'/home/sachaservanschreiber/star-spdz/HOSTS'])
	time_elapsed = (time.time() - time_start)
	time_ttest_5000.append(time_elapsed)

print("===================================")
print("t-test 5,000: " + str(time_ttest_5000))
print("===================================")

for i in range(num_trials):
	time_start = time.time()
	subprocess.call(['./client.x', '2', '0', '--pearson', '2', '0', '1', '0', \
		'/home/sachaservanschreiber/star-spdz/HOSTS'])
	time_elapsed = (time.time() - time_start)
	time_pearson_5000.append(time_elapsed)

print("===================================")
print("pearson 5,000: " + str(time_pearson_5000))
print("===================================")

for i in range(num_trials):
	time_start = time.time()
	subprocess.call(['./client.x', '2', '0', '--ftest', '2', '0', '1', '0', \
		'/home/sachaservanschreiber/star-spdz/HOSTS'])
	time_elapsed = (time.time() - time_start)
	time_ftest_5000.append(time_elapsed)

print("===================================")
print("f-test 5,000: " + str(time_ftest_5000))
print("===================================")


# upload continuous dataset #10000
subprocess.call(['./upload-client.x',\
	'/home/sachaservanschreiber/star-spdz/datasets/patient_continuous_10000.txt', \
	'2','0', '/home/sachaservanschreiber/star-spdz/HOSTS'])

for i in range(num_trials):
	time_start = time.time()
	subprocess.call(['./client.x', '2', '0', '--ttest', '2', '0', '1', '0', \
		'/home/sachaservanschreiber/star-spdz/HOSTS'])
	time_elapsed = (time.time() - time_start)
	time_ttest_10000.append(time_elapsed)

print("===================================")
print("t-test 10,000: " + str(time_ttest_10000))
print("===================================")

for i in range(num_trials):
	time_start = time.time()
	subprocess.call(['./client.x', '2', '0', '--pearson', '2', '0', '1', '0', \
		'/home/sachaservanschreiber/star-spdz/HOSTS'])
	time_elapsed = (time.time() - time_start)
	time_pearson_10000.append(time_elapsed)

print("===================================")
print("pearson 10,000: " + str(time_pearson_10000))
print("===================================")

for i in range(num_trials):
	time_start = time.time()
	subprocess.call(['./client.x', '2', '0', '--ftest', '2', '0', '1', '0', \
		'/home/sachaservanschreiber/star-spdz/HOSTS'])
	time_elapsed = (time.time() - time_start)
	time_ftest_10000.append(time_elapsed)

print("===================================")
print("f-test 10,000: " + str(time_ftest_10000))
print("===================================")
