import datetime
from typing import Union

from auth import get_computer_by_id
from modles import User


def get_folder_mod_date(comp_id: int) -> Union[datetime.date, None]:
    """Return the midification date of the folder"""
    
    user: User = get_computer_by_id(comp_id).user
    return user.backup_date