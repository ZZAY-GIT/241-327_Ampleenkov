from sqlalchemy import String
from datetime import date
from sqlalchemy.orm import relationship, Mapped, mapped_column
from sqlalchemy.ext.declarative import declarative_base

Base = declarative_base()


class Device(Base):
    __tablename__ = "devices"

    id: Mapped[int] = mapped_column(primary_key=True, autoincrement=True)

    name: Mapped[str] = mapped_column(String(120))
    purchase_date: Mapped[date | None] = mapped_column(nullable=True)
    price: Mapped[float | None] = mapped_column(nullable=True)
    is_wireless: Mapped[bool] = mapped_column(default=False)
    condition: Mapped[str] = mapped_column(String(20), default="new")



