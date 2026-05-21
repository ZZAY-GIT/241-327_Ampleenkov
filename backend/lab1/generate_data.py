from faker import Faker
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
from .settings import settings as _settings
from datetime import date, timedelta
import random
from .models import Device

engine = create_engine(_settings.CONN_STR)
Session = sessionmaker(bind=engine)
session = Session()
fake = Faker('ru_RU')


def generate_data():
    """Просто заполняет БД 100 тестовыми записями"""
    
    devices = []
    for i in range(100):
        device = Device(
            name=fake.word().capitalize() + " " + fake.word(),
            purchase_date=date.today() - timedelta(days=random.randint(0, 365*3)),
            price=random.uniform(1000, 50000),
            is_wireless=random.choice([True, False]),
            condition=random.choice(['new', 'used', 'refurbished'])
        )
        devices.append(device)
    
    session.add_all(devices)
    session.commit()
    print(f"✅ Добавлено 100 записей")

if __name__ == "__main__":
    generate_data()