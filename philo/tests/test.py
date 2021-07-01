import os
import subprocess

RED = "\033[31m"
GREEN = "\033[32m"
RESET = "\033[0m"

def test_error_codes():
    cmds = [
            ("./philo", -1),
            ("./philo 2 2 3 4", 0),
            ("./philo 2 2 3 4 5", 0),
            ("./philo 2 2 3 test 5", -1),
            ("./philo 2 -5 2 9", -1),
            ("./philo 2 3 4 5 6 7", -1),
            ]


    print("Testing error codes")
    passed = 0;
    total = 0;
    for cmd in cmds:
        total += 1
        header = f"{cmd[0]}"
        header += " " * (30 - len(header))
        header += f"expecting [{cmd[1]}]"
        print(header + "." * (45 - len(header)), end="")
        try:
            result = subprocess.check_output(cmd[0], shell=True, text=True)
            if cmd[1] == 0:
                print(f"{GREEN}PASSED{RESET} (No Error Code)")
                passed += 1
            else:
                print(f"{RED}FAILED{RESET} (No Error Code")

        except subprocess.CalledProcessError as e:
            if cmd[1] == -1:
                print(f"{GREEN}PASSED{RESET} (Returned Error Code)")
                passed += 1
            else:
                print(f"{RED}FAILED{RESET} (Returned Error Code)")
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

    print(f"\nScore: {passed}/{total}")
