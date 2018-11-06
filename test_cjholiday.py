import collections
import datetime
import functools
import itertools
import operator

import pytest

import cjholiday

# 国民の祝日に関する法律の施行日
MIN_DATE = datetime.date(1948, 7, 20)
# cjholiday における春分の日、秋分の日の予測を試みる最後の年は 2150 年
MAX_DATE = datetime.date(2150, 12, 31)


def range_date(start, stop, step=datetime.timedelta(1)):
    """datetime.date 版 range のようなもの"""
    date = start
    while date < stop:
        yield date
        date += step


def fetch_nth_monday(year, month, n):
    """第 N 月曜日を得る"""
    weekday_month_count = 0
    s = datetime.date(year, month, 1)
    e = datetime.date(year, month + 1, 1)
    for date in range_date(s, e):
        if date.weekday() == 0:
            weekday_month_count += 1
        if weekday_month_count >= n:
            break
    return date


@pytest.fixture
def dates():
    # 法律施行から 1 年以上遡って確認
    start = datetime.date(MIN_DATE.year - 1, 1, 1)
    # 春分の日、秋分の日の予測をやめる年の翌年まで余裕を持って確認
    end = datetime.date(MAX_DATE.year + 2, 1, 1)

    return range_date(start, end)


@pytest.fixture
def years():
    # dates 同様、前後に余裕をもたせる
    return range(MIN_DATE.year - 1, MAX_DATE.year + 2)


def test_args(dates):
    """名前付き引数の使用時の動作確認"""
    holiday_name = cjholiday.holiday_name

    for date in dates:
        s = holiday_name(date.year, date.month, date.day)
        assert holiday_name(year=date.year,
                            month=date.month,
                            day=date.day) == s
        assert holiday_name(date=date) == s


@pytest.mark.xfail()
def test_diff_jholiday(dates):
    """移植元との比較"""
    jholiday = pytest.importorskip('jholiday')

    j = jholiday.holiday_name
    c = cjholiday.holiday_name

    for date in dates:
        assert \
            j(date.year, date.month, date.day) == \
            c(date.year, date.month, date.day)


def test_diff_jholidaydict(dates):
    """拙作 jholidaydict との比較"""
    jholidaydict = pytest.importorskip('jholidaydict')

    jholiday_ins = jholidaydict.JHoliday()
    j = jholiday_ins.get
    c = cjholiday.holiday_name

    for date in jholiday_ins.iter_all_dates():
        assert j(date) == c(date=date)


def test_2018():
    """「平成30年(2018)暦要項」との比較"""
    holiday_name = cjholiday.holiday_name

    assert holiday_name(2018, 1, 1) == '元日'
    assert holiday_name(2018, 1, 8) == '成人の日'
    assert holiday_name(2018, 2, 11) == '建国記念の日'
    assert holiday_name(2018, 3, 21) == '春分の日'
    assert holiday_name(2018, 4, 29) == '昭和の日'
    assert holiday_name(2018, 5, 3) == '憲法記念日'
    assert holiday_name(2018, 5, 4) == 'みどりの日'
    assert holiday_name(2018, 5, 5) == 'こどもの日'
    assert holiday_name(2018, 7, 16) == '海の日'
    assert holiday_name(2018, 8, 11) == '山の日'
    assert holiday_name(2018, 9, 17) == '敬老の日'
    assert holiday_name(2018, 9, 23) == '秋分の日'
    assert holiday_name(2018, 10, 8) == '体育の日'
    assert holiday_name(2018, 11, 3) == '文化の日'
    assert holiday_name(2018, 11, 23) == '勤労感謝の日'
    assert holiday_name(2018, 12, 23) == '天皇誕生日'

    assert holiday_name(2018, 2, 12) == '振替休日'
    assert holiday_name(2018, 4, 30) == '振替休日'
    assert holiday_name(2018, 9, 24) == '振替休日'
    assert holiday_name(2018, 12, 24) == '振替休日'


def test_duplicated(dates):
    """重複確認、同じ年に同じ祝日はない"""
    holiday_name = cjholiday.holiday_name
    ignore = {'振替休日', '国民の休日'}
    contains = functools.partial(operator.contains, ignore)

    for year, dates_ in itertools.groupby(dates, operator.attrgetter('year')):
        holidays = (holiday_name(date=date) for date in dates_)
        holidays = filter(None, holidays)
        holidays = itertools.filterfalse(contains, holidays)
        c = collections.Counter(holidays)
        assert max(c.values(), default=0) <= 1


def test_ganjitsu(years):
    s = '元日'
    t = functools.partial(operator.eq, s)
    f = functools.partial(operator.ne, s)

    holiday_name = cjholiday.holiday_name

    for year in years:
        # 昭和23年(1948年)7月20日から存在、1月1日
        p0101 = t if year > 1948 else f
        assert p0101(holiday_name(year, 1, 1))


def test_seijinnohi(years):
    s = '成人の日'
    t = functools.partial(operator.eq, s)
    f = functools.partial(operator.ne, s)

    holiday_name = cjholiday.holiday_name

    for year in years:
        if year <= 1948:
            p0115, p01m2 = f, f
        elif year < 2000:
            # 昭和23年(1948年)7月20日から存在、1月15日
            p0115, p01m2 = t, f
        else:
            # 平成12年(2000年)から1月第2月曜
            p0115, p01m2 = f, t

        m = fetch_nth_monday(year, 1, n=2)
        assert p0115(holiday_name(year, 1, 15))
        assert p01m2(holiday_name(date=m))


def test_kenkokukinennohi(years):
    s = '建国記念の日'
    t = functools.partial(operator.eq, s)
    f = functools.partial(operator.ne, s)

    holiday_name = cjholiday.holiday_name

    for year in years:
        # 昭和41年(1966年)12月09日から存在、2月11日
        p0211 = t if year > 1966 else f
        assert p0211(holiday_name(year, 2, 11))


def test_shunbunnohi(years):
    s = '春分の日'

    holiday_name = cjholiday.holiday_name

    for year in years:
        # 昭和23年(1948年)7月20日から存在
        num = 1 if year > 1948 and year <= 2150 else 0
        assert (holiday_name(year, 3, 19),
                holiday_name(year, 3, 20),
                holiday_name(year, 3, 21),
                # 3月22日の値を取りうるのは1923年以前および2300年以降。
                # このライブラリの対応期間外ではある
                holiday_name(year, 3, 22)).count(s) == num


def test_showanohi(years):
    s = '昭和の日'
    t = functools.partial(operator.eq, s)
    f = functools.partial(operator.ne, s)

    holiday_name = cjholiday.holiday_name

    for year in years:
        # 平成19年(2007年)から存在、4月29日
        p0429 = t if year >= 2007 else f
        assert p0429(holiday_name(year, 4, 29))


def test_kenpokinenbi(years):
    s = '憲法記念日'
    t = functools.partial(operator.eq, s)
    f = functools.partial(operator.ne, s)

    holiday_name = cjholiday.holiday_name

    for year in years:
        # 昭和23年(1948年)7月20日から存在、5月3日
        p0503 = t if year > 1948 else f
        assert p0503(holiday_name(year, 5, 3))


def test_midorinohi(years):
    s = 'みどりの日'
    t = functools.partial(operator.eq, s)
    f = functools.partial(operator.ne, s)

    holiday_name = cjholiday.holiday_name

    for year in years:
        if year < 1989:
            p0429, p0504 = f, f
        elif year < 2007:
            # 平成元年(1989年)2月17日から存在、4月29日
            p0429, p0504 = t, f
        else:
            # 平成19年(2007年)年から5月4日
            p0429, p0504 = f, t

        assert p0429(holiday_name(year, 4, 29))
        assert p0504(holiday_name(year, 5, 4))


def test_kodomonohi(years):
    s = 'こどもの日'
    t = functools.partial(operator.eq, s)
    f = functools.partial(operator.ne, s)

    holiday_name = cjholiday.holiday_name

    for year in years:
        # 昭和23年(1948年)7月20日から存在、5月5日
        p0505 = t if year > 1948 else f
        assert p0505(holiday_name(year, 5, 5))


def test_uminohi(years):
    s = '海の日'
    t = functools.partial(operator.eq, s)
    f = functools.partial(operator.ne, s)

    holiday_name = cjholiday.holiday_name

    for year in years:
        m3 = fetch_nth_monday(year, 7, n=3)

        table = {20: f, 23: f}
        if year < 1996:
            table[m3.day] = f
        elif year < 2003:
            # 平成8年(1996年)から存在、7月20日
            table[m3.day] = f
            table[20] = t
        elif year < 2020:
            # 平成15年(2003年)年から7月第3月曜
            table[m3.day] = t
        elif year == 2020:
            # 2020年だけ7月23日
            table[m3.day] = f
            table[23] = t
        else:
            table[m3.day] = t

        for day, p in table.items():
            assert p(holiday_name(year, 7, day))


def test_yamanohi(years):
    s = '山の日'
    t = functools.partial(operator.eq, s)
    f = functools.partial(operator.ne, s)

    holiday_name = cjholiday.holiday_name

    for year in years:
        if year < 2016:
            p0811, p0810 = f, f
        elif year < 2020:
            # 平成28年(2016年)から存在、8月11日
            p0811, p0810 = t, f
        elif year == 2020:
            # 2020年だけ8月10日
            p0811, p0810 = f, t
        else:
            p0811, p0810 = t, f

        assert p0811(holiday_name(year, 8, 11))
        assert p0810(holiday_name(year, 8, 10))


def test_keironohi(years):
    s = '敬老の日'
    t = functools.partial(operator.eq, s)
    f = functools.partial(operator.ne, s)

    holiday_name = cjholiday.holiday_name

    for year in years:
        m3 = fetch_nth_monday(year, 9, n=3)

        table = {15: f}
        if year < 1966:
            table[m3.day] = f
        elif year < 2003:
            # 昭和41年(1966年)06月25日から存在、9月15日
            table[m3.day] = f
            table[15] = t
        else:
            # 平成15年(2003年)年から9月第3月曜
            table[m3.day] = t

        for day, p in table.items():
            assert p(holiday_name(year, 9, day))


def test_shuubunnohi(years):
    s = '秋分の日'

    holiday_name = cjholiday.holiday_name

    for year in years:
        # 昭和23年(1948年)7月20日から存在
        num = 1 if year >= 1948 and year <= 2150 else 0
        assert (holiday_name(year, 9, 22),
                holiday_name(year, 9, 23),
                holiday_name(year, 9, 24)).count(s) == num


def test_taikunohi(years):
    s = '体育の日'
    t = functools.partial(operator.eq, s)
    f = functools.partial(operator.ne, s)

    holiday_name = cjholiday.holiday_name

    for year in years:
        m2 = fetch_nth_monday(year, 10, n=2)

        table = {10: f}
        if year < 1966:
            table[m2.day] = f
        elif year < 2000:
            # 昭和41年(1966年)06月25日から存在、10月10日
            table[m2.day] = f
            table[10] = t
        elif year < 2020:
            # 平成12年(2000年)から10月第2月曜
            table[m2.day] = t
        else:
            # 2020年にスポーツの日と入れ替え
            table[m2.day] = f

        for day, p in table.items():
            assert p(holiday_name(year, 10, day))


def test_bunkanohi(years):
    s = '文化の日'
    t = functools.partial(operator.eq, s)
    f = functools.partial(operator.ne, s)

    holiday_name = cjholiday.holiday_name

    for year in years:
        # 昭和23年(1948年)7月20日から存在、11月3日
        p1103 = t if year >= 1948 else f
        assert p1103(holiday_name(year, 11, 3))


def test_kinrokanshanohi(years):
    s = '勤労感謝の日'
    t = functools.partial(operator.eq, s)
    f = functools.partial(operator.ne, s)

    holiday_name = cjholiday.holiday_name

    for year in years:
        # 昭和23年(1948年)7月20日から存在、11月23日
        p1123 = t if year >= 1948 else f
        assert p1123(holiday_name(year, 11, 23))


def test_tennotanjobi(years):
    s = '天皇誕生日'
    t = functools.partial(operator.eq, s)
    f = functools.partial(operator.ne, s)

    holiday_name = cjholiday.holiday_name

    for year in years:
        if year <= 1948:
            p0429, p1223, p0223 = f, f, f
        elif year < 1989:
            # 昭和23年(1948年)7月20日から存在。平成元年(1989年)2月17日まで4月29日
            p0429, p1223, p0223 = t, f, f
        elif year < 2019:
            # 2019年5月1日まで12月23日
            p0429, p1223, p0223 = f, t, f
        elif year == 2019:
            # 2019年12月23日と2019年2月23日はともに違う
            p0429, p1223, p0223 = f, f, f
        else:
            # 2019年5月1日から2月23日
            p0429, p1223, p0223 = f, f, t

        assert p0429(holiday_name(year, 4, 29))
        assert p1223(holiday_name(year, 12, 23))
        assert p0223(holiday_name(year, 2, 23))


def test_supotsunohi(years):
    s = 'スポーツの日'
    t = functools.partial(operator.eq, s)
    f = functools.partial(operator.ne, s)

    holiday_name = cjholiday.holiday_name

    for year in years:
        if year < 2020:
            p0724, p10m2 = f, f
        elif year == 2020:
            # 2020年から存在、この年だけ7月24日
            p0724, p10m2 = t, f
        else:
            # 2021年から10月第2月曜
            p0724, p10m2 = f, t

        m2 = fetch_nth_monday(year, 10, n=2)
        assert p0724(holiday_name(year, 7, 24))
        assert p10m2(holiday_name(date=m2))
