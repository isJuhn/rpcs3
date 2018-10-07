#include "stdafx.h"
#include "Emu/System.h"
#include "Emu/IdManager.h"
#include "Emu/Cell/PPUModule.h"
#include "../Utilities/bin_patch.h"

#include "patchModule.h"

LOG_CHANNEL(patchModule);

vm::ptr<void> do_stuff(ppu_thread& ppu, vm::ptr<void> spawner, vm::ptr<Vec4> arg2, u32 arg3, u32 arg4, vm::ptr<Vec4> arg5)
{
	vm::ptr<SpawnMobyFuncPtr> func_ptr{ vm::addr_t{ 0xBD9288 } };
	vm::ptr<void> crate = func_ptr(ppu, 0x1f4, 0x80);
	patchModule.todo("do_stuff(moby=*0x%x)", crate);
	*(be_t<f32>*)vm::base(crate.addr() + 0x10) = arg2->X;
	*(be_t<f32>*)vm::base(crate.addr() + 0x14) = arg2->Y;
	*(be_t<f32>*)vm::base(crate.addr() + 0x18) = arg2->Z;
	*(be_t<f32>*)vm::base(crate.addr() + 0x1C) = arg2->W;
	*(u8*)vm::base(crate.addr() + 0x30) = 0xFF;
	//*(u8*)vm::base(crate.addr() + 0x31) = 1;
	vm::write8(crate.addr() + 0x31, 1);
	*(u8*)vm::base(crate.addr() + 0x20) = 0;
	*(be_t<u16>*)vm::base(crate.addr() + 0x34) |= 0x100;
	*(be_t<u16>*)vm::base(crate.addr() + 0x32) = 0xFF;
	vm::ptr<void> anim_data = *(vm::ptr<void>*)vm::base(crate.addr() + 0x68);
	vm::ptr<BD5A98> func2{ vm::addr_t{ 0xBD5A98 } };
	func2(ppu, crate);
	return crate;
}

void do_stuff_l(ppu_thread& ppu, u32 moby, u32 anim_data_size)
{
	patchModule.todo("create_moby(mobyID=0x%x, anim_data_size=0x%x)", moby, anim_data_size);
	if (moby == 0xa82)
	{
		ppu.gpr[3] = 0x1867;
		ppu.gpr[4] = 0x300;
	}
	FUNCL_RETURN;
}

void do_nothing(ppu_thread& ppu)
{
	patchModule.todo("do_nothing called from 0x%x", ppu.cia - 0x10);
}

DECLARE(ppu_module_manager::patchModule)("patchModule", []()
{
	REG_FUNC(patchModule, do_stuff);
	REG_FUNC(patchModule, do_stuff_l);
	REG_FUNC(patchModule, do_nothing);
});