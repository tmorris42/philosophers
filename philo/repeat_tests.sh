rm -f output.log
for VAR in {1..100}
do
    # python3.7 tests/test.py > /dev/null;
    echo Starting "#$VAR"
    ./philo 4 410 200 200 10 >> output.log
done
echo -n "output: "
cat output.log | wc
echo -n "Tests with deaths: "
cat output.log | grep died | wc -l