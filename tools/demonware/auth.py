"""Store auth records for an operator-controlled MW3 service."""

from base64 import b64encode
import json
import struct

from .lobby210 import TICKET_SIZE, TITLE

_CLIENT_TICKET = struct.Struct("<IBIIIQQ64s24s7x")


def encode_auth_reply(iv_seed: int, account_id: int, display_name: str,
                      session_key: bytes, server_ticket: bytes) -> bytes:
    """Build a reply for an account and tickets issued by the emulated service."""
    name = display_name.encode("utf-8")
    if not account_id or not name or b"\0" in name or len(name) >= 64:
        raise ValueError("invalid service profile")
    if len(session_key) != 24 or len(server_ticket) != TICKET_SIZE:
        raise ValueError("invalid service ticket material")
    client_ticket = _CLIENT_TICKET.pack(0xefbdadde, 0, TITLE, 0, 0, 0, account_id, name, session_key)
    return json.dumps({
        "auth_task": 45,
        "code": 700,
        "iv_seed": iv_seed,
        "client_ticket": b64encode(client_ticket).decode("ascii"),
        "server_ticket": b64encode(server_ticket).decode("ascii"),
        "extra_data": '{"sbx":"RETAIL","dpi":"MW3"}',
    }, separators=(",", ":")).encode("utf-8")
