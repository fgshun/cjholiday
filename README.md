# 祝日判定

[AddinBox](http://addinbox.sakura.ne.jp/holiday_logic.htm) のコードを Python C 拡張として書き直したものです。

2017/10/1 に Ubuntu 16.04 LTS, Python 3.6.2 でビルド、動作確認しました。かつて 3.3 で動いておりそれから変わっていないはずなので 3.3 以降であればビルドできるはずですが未検証です。

Python 版との違いは holiday\_name にキーワード引数 date が追加されており datetime.date をそのまま渡せるようになっている、というところです。

```
>>> import cjholiday
>>> cjholiday.holiday_name(2017, 8, 11)
'山の日'
>>> import datetime
>>> cjholiday.holiday_name(date=datetime.date(2017, 8, 11))
'山の日'
```
