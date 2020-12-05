import datetime
from argparse import ArgumentParser

from .cjholiday import holiday_name, version


def main() -> None:
    parser = ArgumentParser(prog='cjholiday')
    parser.add_argument('date', type=datetime.date.fromisoformat)
    parser.add_argument('--version',
                        action='version',
                        version='%(prog)s {}'.format(version))

    args = parser.parse_args()
    holiday = holiday_name(date=args.date)
    print(holiday if holiday else '')


if __name__ == '__main__':
    main()
