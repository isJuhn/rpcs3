#pragma once

struct Vec4
{
	be_t<f32> X;
	be_t<f32> Y;
	be_t<f32> Z;
	be_t<f32> W;

	Vec4& add_vec4(Vec4& arg);
	Vec4& sub_vec4(Vec4& arg);
	Vec4& mul_i(f32 arg);
	void operator+=(Vec4& arg);
	be_t<f32> len();
	Vec4& div_i(f32 arg);
	Vec4& norm();
};

struct physgun_mod_data
{
	u8 physics;
	vm::ptr<void> selected_moby;
	Vec4 ray_norm_vector;
	f32 ray_length;
	Vec4 ratchet_pos;
	Vec4 moby_offset;
};

using SpawnMobyFuncPtr = vm::ptr<void>(int moby_id, int pvars_size);
using BD5A98 = void(vm::ptr<void> moby);
using spawn_ray_func = void(vm::ptr<Vec4> arg1, vm::ptr<Vec4> arg2, vm::ptr<Vec4> arg3, u32 arg4, f64 argf1);
using CollLine = b8(vm::ptr<Vec4> vec_from, vm::ptr<Vec4> vec_to, u32, vm::ptr<void> moby, u32);
using sub_2AA928 = void(vm::ptr<Vec4> out, vm::ptr<Vec4> pos, f32, f32, f32);

#define FUNCL_RETURN ppu.ctr = ppu.lr; \
ppu.lr = fxm::get<patch_engine>()->get_HLE_rets()[ppu.lr - 0x8 * 4]


/*
struct Moby
{
	be_t<f32> X;
	be_t<f32> Y;
	be_t<f32> Z;
	be_t<f32> W;
	Vec4 vec4;
	u8 behaviour;
	u8 unk_0x21;
	u8 texture_mode;
	u8 Opacity;
	vm::ptr<void> moby_type;
	vm::ptr<Moby> parent_moby;
	be_t<f32> size;
	u8 unk_0x30;
	u8 visible;
	be_t<u16> render_distance;
	be_t<u32> unk_0x34;
	be_t<u32> unk_0x38;
	u8 color_r;
	u8 color_g;
	u8 color_b;
	u8 unk_0x3F;
};*/
