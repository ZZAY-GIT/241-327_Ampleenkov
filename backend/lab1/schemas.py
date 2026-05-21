from pydantic import BaseModel
from typing import Optional
from datetime import date


class DeviceBase(BaseModel):
    name: str
    purchase_date: Optional[date] = None
    price: Optional[float] = None
    is_wireless: bool = False
    condition: str = "new"

class DeviceCreate(DeviceBase):
    pass

class DeviceUpdate(BaseModel):
    name: Optional[str] = None
    purchase_date: Optional[date] = None
    price: Optional[float] = None
    is_wireless: Optional[bool] = None
    condition: Optional[str] = None

class DeviceResponse(DeviceBase):
    id: int
    
    class Config:
        from_attributes = True