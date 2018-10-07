#pragma once

using SpawnMobyFuncPtr = vm::ptr<void>(int moby_id, int animation_data_size);
using BD5A98 = void(vm::ptr<void> moby);

#define FUNCL_RETURN ppu.ctr = ppu.lr; \
ppu.lr = fxm::get<patch_engine>()->get_HLE_rets()[ppu.lr - 0x8 * 4]

struct Vec4
{
	be_t<f32> X;
	be_t<f32> Y;
	be_t<f32> Z;
	be_t<f32> W;
};
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
