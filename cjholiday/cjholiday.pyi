from datetime import date
from typing import Optional

version: str


def holiday_name(year: Optional[int] = ...,
                 month: Optional[int] = ...,
                 day: Optional[int] = ...,
                 date: Optional[date] = ...) -> Optional[str]:
    ...
