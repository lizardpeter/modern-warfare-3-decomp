"""A model of the Windows desktop for differential tests of the PC layer (win32/, the mouse in client_mp/).

One window: its rectangle on the screen, its client area (origin in screen coordinates, size), the cursor, the
foreground window and ShowCursor's display count. A test builds a Desktop in its input setup (desktop.reset(...)),
and the models below answer the user32 calls from it. Every call is recorded among the machine's observed calls as
("user32", name, arguments...), so the original and the reconstruction must make the same calls, with the same
arguments, in the same order; structures written back (POINT, RECT) are written as the program would write them.
"""
import struct

from unicorn import x86_const as X

ARGS = (X.UC_X86_REG_RCX, X.UC_X86_REG_RDX, X.UC_X86_REG_R8, X.UC_X86_REG_R9)
HWND = 0x00010234  # the game window's handle in the tests (g_wv.hWnd)


def _arg(mu, i):
    return mu.reg_read(ARGS[i])


def _i32(v):
    v &= 0xFFFFFFFF
    return v - (1 << 32) if v >> 31 else v


class Desktop:
    def __init__(self):
        self.reset()

    def reset(self, foreground=HWND, cursor=(0, 0), window=(100, 50, 1380, 770), client=(108, 81, 1264, 680),
              show_count=0):
        """window: (left, top, right, bottom) on the screen; client: (screen x, screen y, width, height)."""
        self.foreground, self.cursor, self.window, self.client = foreground, list(cursor), window, client
        self.show_count = show_count

    def models(self):
        """{import: (fn(machine, mu), 0)} for a case's "imports"."""
        names = {"GetForegroundWindow": self.get_foreground_window, "GetCursorPos": self.get_cursor_pos,
                 "SetCursorPos": self.set_cursor_pos, "ScreenToClient": self.screen_to_client,
                 "ClientToScreen": self.client_to_screen, "GetWindowRect": self.get_window_rect,
                 "GetClientRect": self.get_client_rect, "ClipCursor": self.clip_cursor,
                 "ShowCursor": self.show_cursor, "PostMessageA": self.post_message}
        return {f"user32.dll!{n}": (fn, 0) for n, fn in names.items()}

    @staticmethod
    def _log(m, *call):
        m.observed.append(("user32", *call))

    def get_foreground_window(self, m, mu):
        self._log(m, "GetForegroundWindow")
        mu.reg_write(X.UC_X86_REG_RAX, self.foreground)

    def get_cursor_pos(self, m, mu):
        self._log(m, "GetCursorPos")
        m._emulated_write(_arg(mu, 0), struct.pack("<ii", *self.cursor))
        mu.reg_write(X.UC_X86_REG_RAX, 1)

    def set_cursor_pos(self, m, mu):
        x, y = _i32(_arg(mu, 0)), _i32(_arg(mu, 1))
        self._log(m, "SetCursorPos", x, y)
        self.cursor = [x, y]
        mu.reg_write(X.UC_X86_REG_RAX, 1)

    def _point(self, m, mu, call, sign):
        hwnd, p = _arg(mu, 0), _arg(mu, 1)
        x, y = struct.unpack("<ii", bytes(mu.mem_read(p, 8)))
        self._log(m, call, hwnd, x, y)
        # 32-bit arithmetic, as user32's (a position at the int limits wraps)
        m._emulated_write(p, struct.pack("<ii", _i32(x + sign * self.client[0]), _i32(y + sign * self.client[1])))
        mu.reg_write(X.UC_X86_REG_RAX, 1)

    def screen_to_client(self, m, mu):
        self._point(m, mu, "ScreenToClient", -1)

    def client_to_screen(self, m, mu):
        self._point(m, mu, "ClientToScreen", 1)

    def get_window_rect(self, m, mu):
        self._log(m, "GetWindowRect", _arg(mu, 0))
        m._emulated_write(_arg(mu, 1), struct.pack("<4i", *self.window))
        mu.reg_write(X.UC_X86_REG_RAX, 1)

    def get_client_rect(self, m, mu):
        self._log(m, "GetClientRect", _arg(mu, 0))
        m._emulated_write(_arg(mu, 1), struct.pack("<4i", 0, 0, self.client[2], self.client[3]))
        mu.reg_write(X.UC_X86_REG_RAX, 1)

    def clip_cursor(self, m, mu):
        p = _arg(mu, 0)
        self._log(m, "ClipCursor", struct.unpack("<4i", bytes(mu.mem_read(p, 16))) if p else None)
        mu.reg_write(X.UC_X86_REG_RAX, 1)

    def show_cursor(self, m, mu):
        show = _arg(mu, 0) & 0xFFFFFFFF
        self.show_count += 1 if show else -1
        self._log(m, "ShowCursor", show)
        mu.reg_write(X.UC_X86_REG_RAX, self.show_count & 0xFFFFFFFF)

    def post_message(self, m, mu):
        self._log(m, "PostMessageA", _arg(mu, 0), _arg(mu, 1) & 0xFFFFFFFF, _arg(mu, 2), _arg(mu, 3))
        mu.reg_write(X.UC_X86_REG_RAX, 1)
