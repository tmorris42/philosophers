import datetime
import logging
import os
import subprocess

TIME_TO_WAIT_FOR_DEATH = 10 # in seconds

RED = "\033[31m"
GREEN = "\033[32m"
WHITE = "\033[37m"
RESET = "\033[0m"

LOGFILE = datetime.datetime.now().strftime("./logs/%Y-%m-%d-%H-%M-%S.log")

def test_error_codes():
    logger = logging.getLogger("philo_tester")
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

    logger.info(f"{WHITE}Testing error codes{RESET}")
    passed = 0;
    total = 0;
    for cmd in cmds:
        total += 1
        header = f"{cmd[0]}"
        header += " " * (25 - len(header))
        header += f"expecting [{cmd[1]}]"
        to_print = header + "." * (45 - len(header))
        try:
            result = subprocess.check_output(cmd[0], shell=True, stderr=subprocess.STDOUT, text=True)
            if cmd[1] == 0:
                logger.info(f"{to_print}{GREEN}PASSED{RESET} (No Error Code)")
                passed += 1
            else:
                logger.info(f"{to_print}{RED}FAILED{RESET} (No Error Code")

        except subprocess.CalledProcessError as e:
            if cmd[1] == -1:
                logger.info(f"{to_print}{GREEN}PASSED{RESET} (Returned Error Code)")
                passed += 1
            else:
                logger.info(f"{RED}FAILED{RESET} (Returned Error Code)")
    return (passed, total)

def test_dying_philos():
    logger = logging.getLogger("philo_tester")
    base = "./philo "
    cmds = [
            ("2 1 200 10", 1),
            ("4 310 200 100", 1), #from correction
            ("2 1000 2 2", 0),
            ("4 410 200 200", 0), #from correct?
            ("5 800 200 200", 0),
            ("5 800 200 200 7", 0), #also an everyone eats test
            ]

    logger.info(f"{WHITE}Testing death conditions{RESET}")
    passed = 0;
    total = 0;
    for cmd in cmds:
        total += 1
        header = f"{base}{cmd[0]}"
        header += " " * (25 - len(header))
        header += f"expect death"*(cmd[1] > 0)
        to_print = header + "." * (45 - len(header))
        try:
            if (cmd[1]):
                result = subprocess.check_output(base+cmd[0], shell=True, text=True, timeout=(TIME_TO_WAIT_FOR_DEATH))
            else:
                result = subprocess.check_output(base+cmd[0], shell=True, text=True, timeout=TIME_TO_WAIT_FOR_DEATH)
            if "died" in result and cmd[1]:
                logger.info(f"{to_print}{GREEN}PASSED{RESET} (Death!)")
                passed += 1
            elif cmd[1]:
                logger.info(f"{to_print}{RED}FAILED{RESET} (No death)")
                logger.debug("\n" + result)
            elif "died" in result:
                logger.info(f"{to_print}{RED}FAILED{RESET} (Accidental death)")
                logger.debug("\n" + result)
            else:
                logger.info(f"{to_print}{GREEN}PASSED{RESET} (No death!)")
                passed += 1
        except subprocess.CalledProcessError as e:
                logger.info(f"{to_print}{RED}FAILED{RESET} (Returned Error Code)")
        except subprocess.TimeoutExpired as e:
            if cmd[1]:
                logger.info(f"{to_print}{RED}FAILED{RESET} (no death in time)")
            else:
                logger.info(f"{to_print}{GREEN}PASSED{RESET} (no death in time)")
                passed += 1
    return (passed, total)

def test_one_philo():
    logger = logging.getLogger("philo_tester")
    passed = 0
    total = 1
    cmd = "./philo 1 400 100 100"
    logger.info(f"{WHITE}Testing single philosopher death{RESET}")
    cmd += " " * (25 - len(cmd))
#    cmd += f"expect death"*(cmd[1] > 0)
    to_print = cmd + "." * (45 - len(cmd))
    result = subprocess.check_output(cmd, shell=True, text=True, timeout=(TIME_TO_WAIT_FOR_DEATH))
    if "died" not in result:
        logger.info(f"{to_print}{RED}FAILED{RESET} (No death)")
        logger.debug("\n" + result)
    elif "fork" not in result:
        logger.info(f"{to_print}{RED}FAILED{RESET} (Didn't take fork)")
        logger.debug("\n" + result)
    elif len(result.split("\n")) != 3:
        logger.info(f"{to_print}{RED}FAILED{RESET} (too many messages)")
        logger.debug("\n" + result)
    else:
        logger.info(f"{to_print}{GREEN}PASSED{RESET} (took a fork and died)")
        passed += 1
    return (passed, total)

if __name__ == '__main__':
    if not os.path.isdir("logs"):
        try:
            result = subprocess.check_output("mkdir logs", shell=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            print("Could not create logs directory")
            exit()
    logger = logging.getLogger("philo_tester")
    formatter = logging.Formatter("%(message)s")
    file_handler = logging.FileHandler(LOGFILE)
    file_handler.setFormatter(formatter)
    logger.addHandler(file_handler)
    stream_handler = logging.StreamHandler()
    stream_handler.setLevel(logging.INFO)
    stream_handler.setFormatter(formatter)
    logger.addHandler(stream_handler)
    logger.setLevel(logging.DEBUG)
    passed, total = 0, 0

    test_functions = [
            test_error_codes,
            test_one_philo,
            test_dying_philos,
            ]
    for test in test_functions:
        new_passed, new_total = test()
        passed += new_passed
        total += new_total
        logger.info("\n")

    logger.info(f"{WHITE}Score: {GREEN*(passed == total)+RED*(passed != total)}{passed}/{total}{RESET}")
