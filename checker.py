import os
os.system('g++ -o bsim main.cpp bridgesim.cpp bridge.cpp')
input_directory = r'inputs'
output_directory = r'outputs'
out_directory = r'myoutputs'
for filename in os.listdir(input_directory):
		x = os.path.splitext(filename)[0]
		os.system('./bsim < inputs/{}.txt > myoutputs/{}.txt'.format(x, x))

for filename in os.listdir(out_directory):
		print("{}: Checking".format(filename))
		x = os.path.splitext(filename)[0]
		os.system('diff -BwbZE myoutputs/{}.txt outputs/{}.txt'.format(x, x))

# os.system('rm myoutputs/*')
os.system('rm bsim')