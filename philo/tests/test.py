import os
import subprocess

def test_error_codes():
    cmds = [("./philo", -1),
            ("./philo 1 2 3 4", 0),
            ("./philo 1 2 3 4 5", 0),
            ("./philo 1 2 3 test 5", -1),
            ]

    passed = 0;
    total = 0;
    for cmd in cmds:
        total += 1
        header = f"Testing {cmd[0]}, expecting [{cmd[1]}]"
        print(header + "." * (50 - len(header)), end="")
        try:
            result = subprocess.check_output(cmd[0], shell=True, text=True)
            if cmd[1] == 0:
                print("PASSED (No Error Code)")
                passed += 1
            else:
                print("FAILED (No Error Code")

        except subprocess.CalledProcessError as e:
            if cmd[1] == -1:
                print("PASSED (Returned Error Code)")
                passed += 1
            else:
                print("FAILED (Returned Error Code)")
    return (passed, total)

if __name__ == '__main__':
    if not os.path.isdir("logs"):
        try:
            result = subprocess.check_output("mkdir logs", shell=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            print("Could not create logs directory")
            exit()
    passed, total = 0, 0
    
    new_passed, new_total = test_error_codes()
    passed += new_passed
    total += new_total

    print(f"Score: {passed}/{total}")
