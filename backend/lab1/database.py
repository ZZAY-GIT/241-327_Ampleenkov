from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
from typing import List, Optional, Dict, Any
from .models import Device, Base
from .settings import settings as _settings


class DatabaseManager:
    def __init__(self):
        self.engine = create_engine(_settings.CONN_STR)
        self.session = sessionmaker(bind=self.engine)
        self.create_tables()
    
    def create_tables(self):
        Base.metadata.create_all(self.engine)
        self.Device = Device

    def create_device(self, device_data: Dict[str, Any]) -> Device:
        with self.session() as db:
            device = Device(**device_data)
            db.add(device)
            db.commit()
            db.refresh(device)
            return device
    
    def get_device(self, device_id: int) -> Optional[Device]:
        with self.session() as db:
            return db.query(Device).filter(Device.id == device_id).first()

    def get_all_devices(self, skip: int = 0, limit: int = 100) -> List[Device]:
        with self.session() as db:
            return db.query(Device).limit(limit).all()

    def update_device(self, device_id: int, **kwargs) -> bool:
        with self.session() as db:
            device = db.query(Device).filter(Device.id == device_id).first()
            if device:
                for key, value in kwargs.items():
                    if hasattr(device, key):
                        setattr(device, key, value)
                    else:
                        print(f"Warning: {key} is not a valid attribute")
                db.commit()
                db.refresh(device)
                return True
        return False

    def delete_device(self, device_id: int) -> bool:
        with self.session() as db:
            device = db.query(Device).filter(Device.id == device_id).first()
            if device:
                db.delete(device)
                db.commit()
                return True
            return False
    
    

    


db_manager = DatabaseManager()

# if __name__ == "__main__":
