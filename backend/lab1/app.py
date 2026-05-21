from fastapi import FastAPI, HTTPException
from .database import db_manager
from .schemas import DeviceCreate, DeviceResponse, DeviceUpdate


app = FastAPI()


@app.get("/")
def root():
    return {"message": "Hello, service"}


@app.get("/devices")
def devices():
    devices = db_manager.get_all_devices()
    return {"devices": devices}


@app.post("/devices", response_model=DeviceCreate)
def create_devices(device: DeviceCreate):
    return db_manager.create_device(device.model_dump())


@app.get("/devices/{device_id}", response_model=DeviceResponse)
def get_device(device_id: int):
    device = db_manager.get_device(device_id)
    print(device)
    if not device:
        raise HTTPException(status_code=404, detail="Device not found")
    return device


@app.put("/devices/{device_id}", response_model=DeviceResponse)
def update_device(device_id: int, device_update: DeviceUpdate):
    device = db_manager.get_device(device_id)
    if not device:
        raise HTTPException(status_code=404, detail="Device not found")
    update_data = device_update.model_dump(exclude_unset=True)
    updated = db_manager.update_device(device_id, **update_data)
    if updated:
        updated_device = db_manager.get_device(device_id)
        return updated_device
    raise HTTPException(status_code=500, detail="Failed to update device")


@app.delete("/devices/{device_id}")
def delete_device(device_id: int):
    deleted = db_manager.delete_device(device_id)
    if not deleted:
        raise HTTPException(status_code=404, detail="Device not found")
    return {"message": "Device deleted successfully"}



