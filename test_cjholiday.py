import unittest
import datetime

import jholiday
import cjholiday

def range_date(start, stop, step=datetime.timedelta(1)):
    date = start
    while date < stop:
        yield date
        date += step

class HolidayNameTestCase(unittest.TestCase):
    def setUp(self):
        self.start = datetime.date(1946, 1, 1)
        self.end = datetime.date(2151, 1, 1)

    def test_holiday_name(self):
        j = jholiday.holiday_name
        c = cjholiday.holiday_name

        for date in range_date(self.start, self.end):
            self.assertEqual(
                    j(date.year, date.month, date.day),
                    c(date.year, date.month, date.day))
            self.assertEqual(
                    j(date.year, date.month, date.day),
                    c(year=date.year, month=date.month, day=date.day))

    def test_holiday_name_date(self):
        j = jholiday.holiday_name
        c1 = cjholiday.holiday_name_date
        c2 = cjholiday.holiday_name

        for date in range_date(self.start, self.end):
            self.assertEqual(
                    j(date.year, date.month, date.day),
                    c1(date))
            self.assertEqual(
                    j(date.year, date.month, date.day),
                    c2(date=date))

if __name__ == '__main__':
    unittest.main()
