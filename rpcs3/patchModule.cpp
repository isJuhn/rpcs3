#include "stdafx.h"
#include "Emu/System.h"
#include "Emu/IdManager.h"
#include "Emu/Cell/PPUModule.h"

#include "patchModule.h"

LOG_CHANNEL(patchModule);

void do_stuff(ppu_thread& ppu, vm::ptr<char> ptr)
{
	patchModule.todo("do_stuff(ptr=%s)", ptr);
}

DECLARE(ppu_module_manager::patchModule)("patchModule", []()
{
	REG_FUNC(patchModule, do_stuff);
});