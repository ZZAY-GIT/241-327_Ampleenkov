import pathlib

from pydantic import Field, computed_field, BaseModel
from pydantic_settings import BaseSettings, SettingsConfigDict


class Settings(BaseSettings):
    model_config = SettingsConfigDict(
        env_file=".env",
        env_file_encoding="utf-8",
        extra="allow",
        env_prefix="",
    )

    PROJECT_ROOT: pathlib.Path = pathlib.Path(__file__).parent
    POSTGRES_DB: str = Field(default="postgres_db", alias="POSTGRES_DB")
    POSTGRES_USER: str = Field(default="postgres_user", alias="POSTGRES_USER")
    POSTGRES_PASSWORD: str = Field(default="postgres_password", alias="POSTGRES_PASSWORD")
    POSTGRES_HOST: str = Field(default="localhost", alias="POSTGRES_HOST")
    POSTGRES_PORT: str = Field(default="5432", alias="POSTGRES_PORT")

    @computed_field
    @property
    def CONN_STR(self) -> str:
        return (
            f"postgresql+psycopg2://{self.POSTGRES_USER}:{self.POSTGRES_PASSWORD}@"
            f"{self.POSTGRES_HOST}:{self.POSTGRES_PORT}/{self.POSTGRES_DB}"
        )


settings = Settings()


print(settings.model_dump())