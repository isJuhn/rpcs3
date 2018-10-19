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

void do_stuff_l(ppu_thread& ppu, u32 moby, u32 pvars_size)
{
	patchModule.todo("create_moby(mobyID=0x%x, pvars_size=0x%x) called from 0x%x", moby, pvars_size, ppu.lr);/*
	if (moby == 0x7a || moby == 0x1883)
	{
		ppu.gpr[3] = 0x1867;
		ppu.gpr[4] = 0x300;
	}*/
	FUNCL_RETURN;
}

void do_nothing(ppu_thread& ppu)
{
	patchModule.todo("do_nothing called from 0x%x", ppu.cia - 0x10);
}

void log_moby_id(ppu_thread& ppu, vm::ptr<void> moby)
{
	patchModule.todo("log_moby_id(moby=*0x%x, mobyID=0x%x) called from 0x%x", moby, *(be_t<u16>*)vm::base(moby.addr() + 0xAA), ppu.lr);
	FUNCL_RETURN;
}

vm::ptr<void> shock_blaster_shock_ray_spawner(ppu_thread& ppu, vm::ptr<Vec4> arg1, vm::ptr<Vec4> arg2, vm::ptr<Vec4> arg3, u32 arg4, f64 argf1)
{
	patchModule.todo("shock_blaster_shock_ray_spawner(arg1=*0x%x, arg2=*0x%x, arg3=*0x%x, arg4=0x%x, argf1=%f)", arg1, arg2, arg3, arg4, argf1);
	vm::ptr<SpawnMobyFuncPtr> spawn_moby_func_ptr{ vm::addr_t{ 0xBD9288 } };
	vm::ptr<void> ray_moby = spawn_moby_func_ptr(ppu, 0x1883, 0x80);

	if (!ray_moby)
	{
		return ray_moby;
	}

	vm::write8(ray_moby.addr() + 0x30, 0xFF);
	vm::write8(ray_moby.addr() + 0x31, 0x1);
	vm::write8(ray_moby.addr() + 0x20, 0x0);
	vm::write16(ray_moby.addr() + 0x32, 0xFF);
	*(be_t<u16>*)vm::base(ray_moby.addr() + 0x34) |= 0x41;

	vm::ptr<void> pvars = *(vm::ptr<void>*)vm::base(ray_moby.addr() + 0x68);

	f32 arg2_len = std::sqrtf(arg2->X * arg2->X + arg2->Y * arg2->Y + arg2->Z * arg2->Z);

	if (arg2_len != 0.0)
	{
		vm::write32(pvars.addr() + 0x20, arg2->X * (8.0f / 60) / arg2_len);
		vm::write32(pvars.addr() + 0x24, arg2->Y * (8.0f / 60) / arg2_len);
		vm::write32(pvars.addr() + 0x28, arg2->Z * (8.0f / 60) / arg2_len);
	}
	else
	{
		vm::write32(pvars.addr() + 0x20, (be_t<u32>)arg2->X);
		vm::write32(pvars.addr() + 0x24, (be_t<u32>)arg2->Y);
		vm::write32(pvars.addr() + 0x28, (be_t<u32>)arg2->Z);
		vm::write32(pvars.addr() + 0x28, 0);
		vm::write32(pvars.addr() + 0x24, 0);
		vm::write32(pvars.addr() + 0x20, 0);
	}
	vm::write32(pvars.addr() + 0x2C, (be_t<u32>)arg2->W);
	vm::write32(pvars.addr() + 0x0, (be_t<u32>)arg1->X);
	vm::write32(pvars.addr() + 0x4, (be_t<u32>)arg1->Y);
	vm::write32(pvars.addr() + 0x8, (be_t<u32>)arg1->Z);
	vm::write32(pvars.addr() + 0xC, (be_t<u32>)arg1->W);

	vm::write32(ray_moby.addr() + 0x10, (be_t<u32>)arg3->X);
	vm::write32(ray_moby.addr() + 0x14, (be_t<u32>)arg3->Y);
	vm::write32(ray_moby.addr() + 0x18, (be_t<u32>)arg3->Z);
	vm::write32(ray_moby.addr() + 0x1C, (be_t<u32>)arg3->W);
}

void log_shock_blaster_shock_ray_spawner(ppu_thread& ppu, vm::ptr<Vec4> arg1, vm::ptr<Vec4> arg2, vm::ptr<Vec4> arg3, u32 arg4, f64 argf1)
{
	patchModule.todo("arg1->X=%f, \targ1->Y=%f, \targ1->Z=%f, \targ1->W=%f", arg1->X, arg1->Y, arg1->Z, arg1->W);
	patchModule.todo("arg2->X=%f, \targ2->Y=%f, \targ2->Z=%f, \targ2->W=%f", arg2->X, arg2->Y, arg2->Z, arg2->W);
	patchModule.todo("arg3->X=%f, \targ3->Y=%f, \targ3->Z=%f, \targ3->W=%f", arg3->X, arg3->Y, arg3->Z, arg3->W);
	patchModule.todo("argf1=%f", argf1);
	FUNCL_RETURN;
}

void N90_ray(ppu_thread& ppu, vm::ptr<Vec4> N90_pos, vm::ptr<void> N90_moby)
{
	vm::ptr<spawn_ray_func> spawn_ray{ vm::addr_t{ 0xBE6FF0 } };
	vm::ptr<CollLine> coll_line{ vm::addr_t{ 0xBD5790 } };
	//vm::ptr<Vec4> N90_pos = (vm::ptr<Vec4>)vm::cast(N90_moby.addr() + 0x0);
	vm::var<Vec4> N90_copy = vm::make_var(Vec4{ N90_pos->X, N90_pos->Y, N90_pos->Z, 1.0f });

	//const auto pos = fxm::get_always<Vec4>();
	vm::ptr<Vec4> end_pos{ vm::addr_t{0xda3e40} }; //vm::make_var(Vec4{ pos->X, pos->Y, pos->Z, pos->W }); //vm::make_var(Vec4{ 353.702f, 251.744f, 99.9964f, 1.0f }); //{ vm::addr_t{ 0xEE97A0 } };//vm::make_var(Vec42{ N90_pos->X, N90_pos->Y, N90_pos->Z + 3, 1.0f });

	vm::var<Vec4> vec_0 = vm::make_var(Vec4{ 0, 0, 0, 1 });
	//vm::ptr<void> ratchet_moby = *(vm::ptr<void>*)vm::base(u32{ 0xDA4DB0 });
	//coll_line(ppu, N90_copy, end_pos, 0x20, ratchet_moby, 0);

	vm::ptr<void> sel_moby{ *(be_t<vm::addr_t>*)vm::base(u32{ 0xEE9798 }) };
	if (sel_moby)
	{
		//*(be_t<f32>*)vm::base(sel_moby.addr() + 0x2C) *= 1.1f;
		*(be_t<f32>*)vm::base(sel_moby.addr() + 0x18) += 1.0f;
		patchModule.todo("sel_moby=*0x%x, mobyID=0x%x", sel_moby, vm::read16(sel_moby.addr() + 0xAA));
	}

	spawn_ray(ppu, N90_copy, vec_0, end_pos, 1, 0.0);
}

void physgun_tick(ppu_thread& ppu, vm::ptr<void> moby)
{
	vm::ptr<physgun_mod_data> physgun_data(vm::addr_t{::size32(fxm::get<patch_engine>()->get_HLE_rets()) * 0x4 * 13 + fxm::get<patch_engine>()->get_HLE_rets().cbegin()->second});
	vm::ptr<spawn_ray_func> spawn_ray{ vm::addr_t{ 0xBE6FF0 } };
	vm::ptr<CollLine> coll_line{ vm::addr_t{ 0xBD5790 } };
	//vm::ptr<sub_2AA928>{ vm::addr_t{ 0x2AA928 }};
	vm::var<Vec4> vec_0 = vm::make_var(Vec4{ 0, 0, 0, 1 });
	vm::ptr<Vec4> start_pos{};
	start_pos.set(ppu.gpr[28]);

	if (physgun_data->physics > 0)
	{
		--physgun_data->physics;
	}

	if (!*(be_t<u32>*)vm::base(0xEE9334))
	{
		if (vm::read32(vm::read32(0xC1D574) + 0xA0) & 0x28 && !physgun_data->physics)
		{
			if (!physgun_data->selected_moby)
			{
				vm::ptr<Vec4> end_pos{ vm::addr_t{0xDA3E40} };
				*end_pos += end_pos->sub_vec4(*start_pos);
				vm::ptr<void> ratchet_moby{ vm::addr_t{ *(be_t<vm::addr_t>*)vm::base(0xDA4DB0) } };
				coll_line(ppu, start_pos, end_pos, 0x20, ratchet_moby, 0);
				end_pos.set(0xEE97A0);
				vm::ptr<void> sel_moby{ *(be_t<vm::addr_t>*)vm::base(u32{ 0xEE9798 }) };
				if (sel_moby)
				{
					physgun_data->physics = 5;
					physgun_data->selected_moby = sel_moby;
					physgun_data->moby_offset = ((vm::ptr<Vec4>)vm::cast(sel_moby.addr() + 0x10))->sub_vec4(*end_pos);
					Vec4 ray_vector = end_pos->sub_vec4(*((vm::ptr<Vec4>)vm::cast(ratchet_moby.addr() + 0x10)));
					physgun_data->ray_norm_vector = ray_vector.div_i(ray_vector.len());
					physgun_data->ray_length = ray_vector.len();
					physgun_data->ratchet_pos = *((vm::ptr<Vec4>)vm::cast(ratchet_moby.addr() + 0x10));
					//patchModule.todo("sel_moby=*0x%x, mobyID=0x%x", sel_moby, vm::read16(sel_moby.addr() + 0xAA));

				}
				//patchModule.todo("end_pos->X=%f, \tend_pos->Y=%f, \tend_pos->Z=%f, \tend_pos->W=%f", end_pos->X, end_pos->Y, end_pos->Z, end_pos->W);
			}
			else
			{
				physgun_data->physics = 5;
				physgun_data->selected_moby.set(0x0);
			}
		}
	}

	if (physgun_data->selected_moby)
	{
		vm::ptr<void> ratchet_moby{ vm::addr_t{ *(be_t<vm::addr_t>*)vm::base(0xDA4DB0) } };
		//Vec4 diff = ((vm::ptr<Vec4>)vm::cast(ratchet_moby.addr() + 0x10))->sub_vec4(physgun_data->ratchet_pos);
		physgun_data->ratchet_pos = *(vm::ptr<Vec4>)vm::cast(ratchet_moby.addr() + 0x10);
		physgun_data->ray_norm_vector = (((vm::ptr<Vec4>)vm::cast(u32{ 0xDA3E40 }))->sub_vec4(physgun_data->ratchet_pos)).norm();
		(*(vm::ptr<Vec4>)vm::cast(physgun_data->selected_moby.addr() + 0x10)) = physgun_data->ratchet_pos.add_vec4(physgun_data->ray_norm_vector.mul_i(physgun_data->ray_length)).add_vec4(physgun_data->moby_offset);
		spawn_ray(ppu, start_pos, vec_0, vm::make_var(physgun_data->ratchet_pos.add_vec4(physgun_data->ray_norm_vector.mul_i(physgun_data->ray_length)) /*Vec4{ start_pos->X + physgun_data->ray_vector.X, start_pos->Y + physgun_data->ray_vector.Y, start_pos->Z + physgun_data->ray_vector.Z, 1.0f }*/), 1, 0.0f);
	}
	ppu.lr = 0x5A1734;
}

void save_splitter_rifle_shoot(ppu_thread& ppu, vm::ptr<Vec4> pos, vm::ptr<Vec4> vec)
{
	const auto shoot_pos = fxm::get_always<Vec4>();
	shoot_pos->X = pos->X + vec->X;
	shoot_pos->Y = pos->Y + vec->Y;
	shoot_pos->Z = pos->Z + vec->Z;
	shoot_pos->W = 1.0f;
	FUNCL_RETURN;
}

void log_splitter_rifle_bullet_spawner(ppu_thread& ppu, vm::ptr<void> moby, vm::ptr<Vec4> arg2, vm::ptr<Vec4> arg3, vm::ptr<Vec4> arg4, u32 arg5, u32 arg6, u32 arg7, u32 arg8, f32 argf1)
{
	patchModule.todo("moby=*0x%x, mobyID=0x%x", moby, vm::read32(moby.addr() + 0xAA));
	patchModule.todo("arg2->X=%f, \targ2->Y=%f, \targ2->Z=%f, \targ2->W=%f", arg2->X, arg2->Y, arg2->Z, arg2->W);
	patchModule.todo("arg3->X=%f, \targ3->Y=%f, \targ3->Z=%f, \targ3->W=%f", arg3->X, arg3->Y, arg3->Z, arg3->W);
	patchModule.todo("arg4->X=%f, \targ4->Y=%f, \targ4->Z=%f, \targ4->W=%f", arg4->X, arg4->Y, arg4->Z, arg4->W);
	patchModule.todo("arg5=0x%x, \targ6=0x%x, \targ7=0x%x, \targ8=0x%x, \targf1=%f", arg5, arg6, arg7, arg8, argf1);
	FUNCL_RETURN;
}

void ratchet_physics_override(ppu_thread& ppu)
{

}

DECLARE(ppu_module_manager::patchModule)("patchModule", []()
{
	REG_FUNC(patchModule, do_stuff);
	REG_FUNC(patchModule, do_stuff_l);
	REG_FUNC(patchModule, do_nothing);
	REG_FUNC(patchModule, log_moby_id);
	REG_FUNC(patchModule, shock_blaster_shock_ray_spawner);
	REG_FUNC(patchModule, log_shock_blaster_shock_ray_spawner);
	REG_FUNC(patchModule, N90_ray);
	REG_FUNC(patchModule, log_splitter_rifle_bullet_spawner);
	REG_FUNC(patchModule, save_splitter_rifle_shoot);
	REG_FUNC(patchModule, physgun_tick);
});

Vec4& Vec4::add_vec4(Vec4& arg)
{
	Vec4 res{};
	res.X = X + arg.X;
	res.Y = Y + arg.Y;
	res.Z = Z + arg.Z;
	res.W = 1.0f;
	return res;
}

Vec4& Vec4::sub_vec4(Vec4& arg)
{
	Vec4 res{};
	res.X = X - arg.X;
	res.Y = Y - arg.Y;
	res.Z = Z - arg.Z;
	res.W = 1.0f;
	return res;
}

Vec4& Vec4::mul_i(f32 arg)
{
	Vec4 res{};
	res.X = X * arg;
	res.Y = Y * arg;
	res.Z = Z * arg;
	res.W = 1.0f;
	return res;
}

void Vec4::operator+=(Vec4& arg)
{
	X += arg.X;
	Y += arg.Y;
	Z += arg.Z;
}

be_t<f32> Vec4::len()
{
	return sqrt(X * X + Y * Y + Z * Z);
}

Vec4& Vec4::div_i(f32 arg)
{
	Vec4 res{};
	res.X = X / arg;
	res.Y = Y / arg;
	res.Z = Z / arg;
	res.W = 1;
	return res;
}

Vec4& Vec4::norm()
{
	Vec4 res{};
	const auto len = Vec4::len();
	res.X = X / len;
	res.Y = Y / len;
	res.Z = Z / len;
	res.W = 1.0;
	return res;
}