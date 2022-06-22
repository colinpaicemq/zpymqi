touch *.c 
export _C99_CCMODE=1 
# python3 setup.py clean 
python3 setup.py build bdist_wheel 1>a 2>b 
oedit a b 
