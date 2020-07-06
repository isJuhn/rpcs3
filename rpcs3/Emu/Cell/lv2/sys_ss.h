#pragma once

#include "Emu/Memory/vm_ptr.h"
#include "Emu/Cell/ErrorCodes.h"

// Unofficial error code names
enum sys_ss_rng_error : u32
{
	SYS_SS_RNG_ERROR_INVALID_PKG = 0x80010500,
	SYS_SS_RNG_ERROR_ENOMEM = 0x80010501,
	SYS_SS_RNG_ERROR_EAGAIN = 0x80010503,
	SYS_SS_RNG_ERROR_EFAULT = 0x80010509,
};

enum sys_ss_vtrm_packet : u32
{
	SYS_SS_VTRM_PACKET_INIT = 0x2001,
	SYS_SS_VTRM_PACKET_STATUS = 0x2002,
	SYS_SS_VTRM_PACKET_STORE_WITH_TRM_UPDATE = 0x2003,
	SYS_SS_VTRM_PACKET_STORE = 0x2004,
	SYS_SS_VTRM_PACKET_RETRIEVE = 0x2005,
	SYS_SS_VTRM_PACKET_FREE = 0x2006,
	SYS_SS_VTRM_PACKET_ENCRYPT = 0x200A,
	SYS_SS_VTRM_PACKET_DECRYPT = 0x200B,
	SYS_SS_VTRM_PACKET_ENCRYPT_WITH_PORTABILITY = 0x200C,
	SYS_SS_VTRM_PACKET_DECRYPT_WITH_PORTABILITY = 0x200D,
	SYS_SS_VTRM_PACKET_DECRYPT_MASTER = 0x200E,
	SYS_SS_VTRM_PACKET_BACKUP_FLASH = 0x2012,
	SYS_SS_VTRM_PACKET_RESTORE_FLASH = 0x2013,
	SYS_SS_VTRM_PACKET_BACKUP_SRK_SRH = 0x2014,
	SYS_SS_VTRM_PACKET_RESTORE_SRK_SRH = 0x2015,
	SYS_SS_VTRM_PACKET_FLASH_ADDRESS_SIZE = 0x2016,
	SYS_SS_VTRM_PACKET_FORCE_RESTART = 0x2017,
};

struct CellSsOpenPSID
{
	be_t<u64> high;
	be_t<u64> low;
};

error_code sys_ss_random_number_generator(u64 pkg_id, vm::ptr<void> buf, u64 size);
error_code sys_ss_access_control_engine(u64 pkg_id, u64 a2, u64 a3);
s32 sys_ss_get_console_id(vm::ptr<u8> buf);
s32 sys_ss_get_open_psid(vm::ptr<CellSsOpenPSID> ptr);
error_code sys_ss_appliance_info_manager(u32 code, vm::ptr<u8> buffer);
error_code sys_ss_get_cache_of_product_mode(vm::ptr<u8> ptr);
error_code sys_ss_secure_rtc(u64 cmd, u64 a2, u64 a3, u64 a4);
error_code sys_ss_get_cache_of_flash_ext_flag(vm::ptr<u64> flag);
error_code sys_ss_get_boot_device(vm::ptr<u64> dev);
error_code sys_ss_update_manager(u64 pkg_id, u64 a1, u64 a2, u64 a3, u64 a4, u64 a5, u64 a6);
error_code sys_ss_virtual_trm_manager(u64 pkg_id, u64 a1, u64 a2, u64 a3, u64 a4);
error_code sys_ss_individual_info_manager(u64 pkg_id, u64 a2, vm::ptr<u64> out_size, u64 a4, u64, u64 a5);
