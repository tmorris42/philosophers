import os
import subprocess

TIME_TO_WAIT_FOR_DEATH = 30 # in seconds

RED = "\033[31m"
GREEN = "\033[32m"
WHITE = "\033[37m"
RESET = "\033[0m"

def test_error_codes():
    cmds = [
            ("./philo", -1),
            ("./philo 2 2 3 4", 0),
            ("./philo 2 2 3 4 5", 0),
            ("./philo 2 2 3 test 5", -1),
            ("./philo 2 -5 2 9", -1),
            ("./philo 2 -5- 2 9", -1),
            ("./philo 2 5 +2 9", -1),
            ("./philo 2 5 2 a", -1),
            ("./philo 2 3 4 5 6 7", -1),
            ]

    print(f"{WHITE}Testing error codes{RESET}")
    passed = 0;
    total = 0;
    for cmd in cmds:
        total += 1
        header = f"{cmd[0]}"
        header += " " * (25 - len(header))
        header += f"expecting [{cmd[1]}]"
        print(header + "." * (45 - len(header)), end="")
        try:
            result = subprocess.check_output(cmd[0], shell=True, stderr=subprocess.STDOUT, text=True)
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

def test_dying_philos():
    base = "./philo "
    cmds = [
            ("2 1 200 10", 1),
            ("4 310 200 100", 1), #from correction
            ("4 310 200 100", 10),
            ("2 1000 2 2", 0),
            ("4 410 200 200", 0), #from correct?
            ("5 800 200 200", 0),
            ("5 800 200 200 7", 0), #also an everyone eats test
            ]

    print(f"{WHITE}Testing death conditions{RESET}")
    passed = 0;
    total = 0;
    for cmd in cmds:
        total += 1
        header = f"{base}{cmd[0]}"
        header += " " * (25 - len(header))
        header += f"expect death"*(cmd[1] > 0)
        print(header + "." * (45 - len(header)), end="")
        try:
            if (cmd[1]):
                result = subprocess.check_output(base+cmd[0], shell=True, text=True, timeout=cmd[1]+0.01)
            else:
                result = subprocess.check_output(base+cmd[0], shell=True, text=True, timeout=TIME_TO_WAIT_FOR_DEATH)
            if "has died" in result and cmd[1]:
                print(f"{GREEN}PASSED{RESET} (Death!)")
                passed += 1
            elif cmd[1]:
                print(f"{RED}FAILED{RESET} (No death)")
            else:
                print(f"{RED}FAILED{RESET} (Accidental death)")
        except subprocess.CalledProcessError as e:
                print(f"{RED}FAILED{RESET} (Returned Error Code)")
        except subprocess.TimeoutExpired as e:
            if cmd[1]:
                print(f"{RED}FAILED{RESET} (no death in time)")
            else:
                print(f"{GREEN}PASSED{RESET} (no death in time)")
                passed += 1
    return (passed, total)


if __name__ == '__main__':
    if not os.path.isdir("logs"):
        try:
            result = subprocess.check_output("mkdir logs", shell=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            print("Could not create logs directory")
            exit()
    passed, total = 0, 0

    test_functions = [
            test_error_codes,
            test_dying_philos,
            ]
    for test in test_functions:
        new_passed, new_total = test()
        passed += new_passed
        total += new_total
        print("\n")

    print(f"{WHITE}Score: {GREEN*(passed == total)+RED*(passed != total)}{passed}/{total}{RESET}")
