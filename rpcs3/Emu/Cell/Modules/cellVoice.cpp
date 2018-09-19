#include "stdafx.h"
#include "Emu/System.h"
#include "Emu/Cell/PPUModule.h"

#include "Emu/IdManager.h"
#include "Emu/Cell/lv2/sys_event.h"
#include "cellVoice.h"
#include <thread>

LOG_CHANNEL(cellVoice);

template <>
void fmt_class_string<CellVoiceError>::format(std::string& out, u64 arg)
{
	format_enum(out, arg, [](CellVoiceError value)
	{
		switch (value)
		{
			STR_CASE(CELL_VOICE_ERROR_ADDRESS_INVALID);
			STR_CASE(CELL_VOICE_ERROR_ARGUMENT_INVALID);
			STR_CASE(CELL_VOICE_ERROR_CONTAINER_INVALID);
			STR_CASE(CELL_VOICE_ERROR_DEVICE_NOT_PRESENT);
			STR_CASE(CELL_VOICE_ERROR_EVENT_DISPATCH);
			STR_CASE(CELL_VOICE_ERROR_EVENT_QUEUE);
			STR_CASE(CELL_VOICE_ERROR_GENERAL);
			STR_CASE(CELL_VOICE_ERROR_LIBVOICE_INITIALIZED);
			STR_CASE(CELL_VOICE_ERROR_LIBVOICE_NOT_INIT);
			STR_CASE(CELL_VOICE_ERROR_RESOURCE_INSUFFICIENT);
			STR_CASE(CELL_VOICE_ERROR_SERVICE_ATTACHED);
			STR_CASE(CELL_VOICE_ERROR_SERVICE_DETACHED);
			STR_CASE(CELL_VOICE_ERROR_SERVICE_HANDLE);
			STR_CASE(CELL_VOICE_ERROR_SERVICE_NOT_FOUND);
			STR_CASE(CELL_VOICE_ERROR_SHAREDMEMORY);
			STR_CASE(CELL_VOICE_ERROR_TOPOLOGY);
		}

		return unknown;
	});
}

void voice_thread::on_init(const std::shared_ptr<void>& _this)
{
	named_thread::on_init(_this);
}

void voice_thread::on_task()
{
	std::this_thread::sleep_for(1s);
}

s32 cellVoiceConnectIPortToOPort(u32 iPort, u32 oPort)
{
	cellVoice.todo("cellVoiceConnectIPortToOPort(iPort=0x%x, oPort=0x%x)", iPort, oPort);

	const auto voiceThread = fxm::get<voice_thread>();
	if (!voiceThread)
	{
		return CELL_VOICE_ERROR_LIBVOICE_NOT_INIT;
	}

	return CELL_OK;
}

error_code cellVoiceCreateNotifyEventQueue(vm::ptr<u32> id, vm::ptr<u64> key)
{
	cellVoice.todo("cellVoiceCreateNotifyEventQueue(id=*0x%x, key=*0x%x)", id, key);

	const auto voiceThread = fxm::get<voice_thread>();
	if (!voiceThread)
	{
		return CELL_VOICE_ERROR_LIBVOICE_NOT_INIT;
	}

	vm::var<sys_event_queue_attribute_t> attr;
	attr->protocol = SYS_SYNC_FIFO;
	attr->type = SYS_PPU_QUEUE;
	attr->name_u64 = 0;

	for (u64 i = 0; i < 100; i++)
	{
		// Create an event queue "bruteforcing" an available key
		const u64 key_value = 0x80004d494f323221ull + i;

		if (const s32 res = sys_event_queue_create(id, attr, key_value, 32))
		{
			if (res != CELL_EEXIST)
			{
				return res;
			}
		}
		else
		{
			*key = key_value;
			return CELL_OK;
		}
	}

	return CELL_VOICE_ERROR_EVENT_QUEUE;
}

error_code cellVoiceCreatePort(vm::ptr<u32> portId, vm::cptr<CellVoicePortParam> pArg)
{
	cellVoice.todo("cellVoiceCreatePort(portId=*0x%x, pArg=*0x%x)", portId, pArg);
	return CELL_OK;
}

s32 cellVoiceDeletePort()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

s32 cellVoiceDisconnectIPortFromOPort()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

error_code cellVoiceEnd()
{
	cellVoice.todo("cellVoiceEnd()");

	const auto voiceThread = fxm::get<voice_thread>();
	if (!voiceThread)
	{
		return CELL_VOICE_ERROR_LIBVOICE_NOT_INIT;
	}

	return CELL_OK;
	return CELL_OK;
}

s32 cellVoiceGetBitRate()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

s32 cellVoiceGetMuteFlag()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

s32 cellVoiceGetPortAttr()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

s32 cellVoiceGetPortInfo()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

s32 cellVoiceGetSignalState()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

s32 cellVoiceGetVolume()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

error_code cellVoiceInit(vm::ptr<CellVoiceInitParam> pArg)
{
	cellVoice.todo("cellVoiceInit(pArg=*0x%x)", pArg);

	const auto voiceThread = fxm::make<voice_thread>();
	voiceInited = true;
	if (!voiceThread)
	{
		return CELL_VOICE_ERROR_LIBVOICE_INITIALIZED;
	}

	return CELL_OK;
}

s32 cellVoiceInitEx()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

s32 cellVoicePausePort()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

s32 cellVoicePausePortAll()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

s32 cellVoiceRemoveNotifyEventQueue()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

s32 cellVoiceResetPort()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

s32 cellVoiceResumePort()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

s32 cellVoiceResumePortAll()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

error_code cellVoiceSetBitRate(u32 portId, s32 bitrate)
{
	cellVoice.todo("cellVoiceSetBitRate(portId=0x%x, bitrate=%d)", portId, bitrate);

	const auto voiceThread = fxm::get<voice_thread>();
	if (!voiceThread)
	{
		return CELL_VOICE_ERROR_LIBVOICE_NOT_INIT;
	}

	return CELL_OK;
}

s32 cellVoiceSetMuteFlag()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

s32 cellVoiceSetMuteFlagAll()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

error_code cellVoiceSetNotifyEventQueue(u64 key)
{
	cellVoice.todo("cellVoiceSetNotifyEventQueue(key=0x%llx)", key);

	const auto voiceThread = fxm::get<voice_thread>();
	if (!voiceThread)
	{
		return CELL_VOICE_ERROR_LIBVOICE_NOT_INIT;
	}

	voiceThread->eventQueueKey = key;

	return CELL_OK;
}

s32 cellVoiceSetPortAttr()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

s32 cellVoiceSetVolume()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

error_code cellVoiceStart()
{
	cellVoice.todo("cellVoiceStart()");

	const auto voiceThread = fxm::get<voice_thread>();
	if (!voiceThread)
	{
		return CELL_VOICE_ERROR_LIBVOICE_NOT_INIT;
	}

	auto voiceQueue = lv2_event_queue::find(voiceThread->eventQueueKey);
	voiceQueue->send(0, CELLVOICE_EVENT_SERVICE_ATTACHED, 0, 0);

	return CELL_OK;
}

s32 cellVoiceStartEx()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

error_code cellVoiceStop()
{
	cellVoice.todo("cellVoiceStop()");

	const auto voiceThread = fxm::get<voice_thread>();
	if (!voiceThread)
	{
		return CELL_VOICE_ERROR_LIBVOICE_NOT_INIT;
	}

	return CELL_OK;
}

s32 cellVoiceUpdatePort()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

s32 cellVoiceWriteToIPort()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

s32 cellVoiceWriteToIPortEx()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

s32 cellVoiceWriteToIPortEx2()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

s32 cellVoiceReadFromOPort()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

s32 cellVoiceDebugTopology()
{
	UNIMPLEMENTED_FUNC(cellVoice);
	return CELL_OK;
}

DECLARE(ppu_module_manager::cellVoice)("cellVoice", []()
{
	REG_FUNC(cellVoice, cellVoiceConnectIPortToOPort);
	REG_FUNC(cellVoice, cellVoiceCreateNotifyEventQueue);
	REG_FUNC(cellVoice, cellVoiceCreatePort);
	REG_FUNC(cellVoice, cellVoiceDeletePort);
	REG_FUNC(cellVoice, cellVoiceDisconnectIPortFromOPort);
	REG_FUNC(cellVoice, cellVoiceEnd);
	REG_FUNC(cellVoice, cellVoiceGetBitRate);
	REG_FUNC(cellVoice, cellVoiceGetMuteFlag);
	REG_FUNC(cellVoice, cellVoiceGetPortAttr);
	REG_FUNC(cellVoice, cellVoiceGetPortInfo);
	REG_FUNC(cellVoice, cellVoiceGetSignalState);
	REG_FUNC(cellVoice, cellVoiceGetVolume);
	REG_FUNC(cellVoice, cellVoiceInit);
	REG_FUNC(cellVoice, cellVoiceInitEx);
	REG_FUNC(cellVoice, cellVoicePausePort);
	REG_FUNC(cellVoice, cellVoicePausePortAll);
	REG_FUNC(cellVoice, cellVoiceRemoveNotifyEventQueue);
	REG_FUNC(cellVoice, cellVoiceResetPort);
	REG_FUNC(cellVoice, cellVoiceResumePort);
	REG_FUNC(cellVoice, cellVoiceResumePortAll);
	REG_FUNC(cellVoice, cellVoiceSetBitRate);
	REG_FUNC(cellVoice, cellVoiceSetMuteFlag);
	REG_FUNC(cellVoice, cellVoiceSetMuteFlagAll);
	REG_FUNC(cellVoice, cellVoiceSetNotifyEventQueue);
	REG_FUNC(cellVoice, cellVoiceSetPortAttr);
	REG_FUNC(cellVoice, cellVoiceSetVolume);
	REG_FUNC(cellVoice, cellVoiceStart);
	REG_FUNC(cellVoice, cellVoiceStartEx);
	REG_FUNC(cellVoice, cellVoiceStop);
	REG_FUNC(cellVoice, cellVoiceUpdatePort);
	REG_FUNC(cellVoice, cellVoiceWriteToIPort);
	REG_FUNC(cellVoice, cellVoiceWriteToIPortEx);
	REG_FUNC(cellVoice, cellVoiceWriteToIPortEx2);
	REG_FUNC(cellVoice, cellVoiceReadFromOPort);
	REG_FUNC(cellVoice, cellVoiceDebugTopology);
});
