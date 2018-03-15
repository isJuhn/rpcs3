#include "stdafx.h"
#include "Emu/Cell/PPUModule.h"

#include "sceNp.h"
#include "sceNp2.h"
#include "cellSysutil.h"
#include "Emu/IdManager.h"

logs::channel sceNp2("sceNp2");

s32 sceNp2Init(u32 poolsize, vm::ptr<void> poolptr)
{
	sceNp2.warning("sceNp2Init(poolsize=0x%x, poolptr=*0x%x)", poolsize, poolptr);

	if (poolsize == 0)
	{
		return SCE_NP_ERROR_INVALID_ARGUMENT;
	}
	else if (poolsize < 128 * 1024)
	{
		return SCE_NP_ERROR_INSUFFICIENT_BUFFER;
	}

	if (!poolptr)
	{
		return SCE_NP_ERROR_INVALID_ARGUMENT;
	}

	return CELL_OK;
}

s32 sceNpMatching2Init(u32 poolsize, s32 priority)
{
	sceNp2.todo("sceNpMatching2Init(poolsize=0x%x, priority=%d)", poolsize, priority);

	return CELL_OK;
}

s32 sceNpMatching2Init2(u32 poolsize, s32 priority, vm::ptr<SceNpMatching2UtilityInitParam> param)
{
	sceNp2.todo("sceNpMatching2Init2(poolsize=0x%x, priority=%d, param=*0x%x)", poolsize, priority, param);

	// TODO:
	// 1. Create an internal thread
	// 2. Create heap area to be used by the NP matching 2 utility
	// 3. Set maximum lengths for the event data queues in the system

	return CELL_OK;
}

s32 sceNp2Term()
{
	sceNp2.warning("sceNp2Term()");

	return CELL_OK;
}

s32 sceNpMatching2Term(ppu_thread& ppu)
{
	sceNp2.warning("sceNpMatching2Term()");

	return CELL_OK;
}

s32 sceNpMatching2Term2()
{
	sceNp2.warning("sceNpMatching2Term2()");

	return CELL_OK;
}

s32 sceNpMatching2DestroyContext()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2LeaveLobby()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2RegisterLobbyMessageCallback()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2GetWorldInfoList()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2RegisterLobbyEventCallback()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2GetLobbyMemberDataInternalList()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2SearchRoom()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2SignalingGetConnectionStatus()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2SetUserInfo()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2GetClanLobbyId()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2GetLobbyMemberDataInternal()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2ContextStart(SceNpMatching2ContextId ctxId)
{
	sceNp2.todo("sceNpMatching2ContextStart(ctxId=0x%x)", ctxId);

	sysutil_register_cb([=](ppu_thread& ppu) -> s32
	{
		const auto matching2Context = fxm::get_always<SceNpMatching2Context_t>();

		matching2Context->cbFunc(ppu, ctxId, SCE_NP_MATCHING2_CONTEXT_EVENT_Start, SCE_NP_MATCHING2_EVENT_CAUSE_CONTEXT_ACTION, 0, matching2Context->cbFuncArg);
		return CELL_OK;
	});

	return CELL_OK;
}

s32 sceNpMatching2CreateServerContext()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2GetMemoryInfo(vm::ptr<SceNpMatching2MemoryInfo> memInfo)
{
	sceNp2.todo("sceNpMatching2GetMemoryInfo(memInfo=*0x%x)", memInfo);

	memInfo->curMemUsage = 0x2000;
	memInfo->maxMemUsage = 0x40000;
	memInfo->totalMemSize = 0x40000;

	return CELL_OK;
}

s32 sceNpMatching2LeaveRoom()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2SetRoomDataExternal()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2SignalingGetConnectionInfo()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2SendRoomMessage()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2JoinLobby()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2GetRoomMemberDataExternalList()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2AbortRequest()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2GetServerInfo()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2GetEventData()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2GetRoomSlotInfoLocal()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2SendLobbyChatMessage()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2AbortContextStart()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2GetRoomMemberIdListLocal()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2JoinRoom()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2GetRoomMemberDataInternalLocal()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2GetCbQueueInfo()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2KickoutRoomMember()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2ContextStartAsync(SceNpMatching2ContextId ctxId, u32 timeout)
{
	sceNp2.todo("sceNpMatching2ContextStartAsync(ctxId=0x%x, timeout=0x%x)", ctxId, timeout);

	sysutil_register_cb([=](ppu_thread& ppu) -> s32
	{
		const auto matching2Context = fxm::get_always<SceNpMatching2Context_t>();

		matching2Context->cbFunc(ppu, ctxId, SCE_NP_MATCHING2_CONTEXT_EVENT_Start, SCE_NP_MATCHING2_EVENT_CAUSE_CONTEXT_ACTION, 0, matching2Context->cbFuncArg);
		return CELL_OK;
	});

	return CELL_OK;
}

s32 sceNpMatching2SetSignalingOptParam()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2RegisterContextCallback(ppu_thread& ppu, SceNpMatching2ContextId ctxId, vm::ptr<SceNpMatching2ContextCallback> cbFunc, vm::ptr<void> cbFuncArg)
{
	sceNp2.todo("sceNpMatching2RegisterContextCallback(ctxId=0x%x, cbFunc=*0x%x, cbFuncArg=*0x%x)", ctxId, cbFunc, cbFuncArg);

	const auto matching2Context = fxm::get_always<SceNpMatching2Context_t>();
	matching2Context->cbFunc = cbFunc;
	matching2Context->cbFuncArg = cbFuncArg;

	return CELL_OK;
}

s32 sceNpMatching2SendRoomChatMessage()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2SetRoomDataInternal()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2GetRoomDataInternal()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2SignalingGetPingInfo()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2GetServerIdListLocal(SceNpMatching2ContextId ctxId, vm::ptr<SceNpMatching2ServerId> serverId, u32 serverIdNum)
{
	sceNp2.todo("sceNpMatching2GetServerIdListLocal(ctxId=0x%x, serverId=*0x%x, serverIdNum=0x%x)", ctxId, serverId, serverIdNum);
	return CELL_OK;
}

s32 sceNpUtilBuildCdnUrl()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2GrantRoomOwner()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2CreateContext(vm::cptr<SceNpId> npId, vm::cptr<SceNpCommunicationId> commId, vm::cptr<SceNpCommunicationPassphrase> passPhrase, SceNpMatching2ContextId ctxId, s32 option)
{
	sceNp2.todo("sceNpMatching2CreateContext(npId=*0x%x, commId=*0x%x, passPhrase=*0x%x, ctxId=0x%x, option=0x%x)", npId, commId, passPhrase, ctxId, option);
	return CELL_OK;
}

s32 sceNpMatching2GetSignalingOptParamLocal()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2RegisterSignalingCallback(SceNpMatching2ContextId ctxId, vm::ptr<SceNpMatching2SignalingCallback> cbFunc, vm::ptr<void> cbFuncArg)
{
	sceNp2.todo("sceNpMatching2RegisterSignalingCallback(ctxId=0x%x, cbFunc=*0x%x, cbFuncArg=*0x%x)", ctxId, cbFunc, cbFuncArg);

	const auto matching2Signaling = fxm::get_always<SceNpMatching2Signaling_t>();
	matching2Signaling->cbFunc = cbFunc;
	matching2Signaling->cbFuncArg = cbFuncArg;

	return CELL_OK;
}

s32 sceNpMatching2ClearEventData()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2GetUserInfoList()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2GetRoomMemberDataInternal()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2SetRoomMemberDataInternal()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2JoinProhibitiveRoom()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2SignalingSetCtxOpt()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2DeleteServerContext()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2SetDefaultRequestOptParam(SceNpMatching2ContextId ctxId, vm::cptr<SceNpMatching2RequestOptParam> optParam)
{
	sceNp2.todo("sceNpMatching2SetDefaultRequestOptParam(ctxId=0x%x, optParam=*0x%x)", ctxId, optParam);

	fxm::make_always<SceNpMatching2RequestOptParam>(*optParam);

	return CELL_OK;
}

s32 sceNpMatching2RegisterRoomEventCallback(SceNpMatching2ContextId ctxId, vm::ptr<SceNpMatching2RoomEventCallback> cbFunc, vm::ptr<void> cbFuncArg)
{
	sceNp2.todo("sceNpMatching2RegisterRoomEventCallback(ctxId=0x%x, cbFunc=*0x%x, cbFuncArg=*0x%x)", ctxId, cbFunc, cbFuncArg);

	const auto matching2RoomEvent = fxm::get_always<SceNpMatching2RoomEvent_t>();
	matching2RoomEvent->cbFunc = cbFunc;
	matching2RoomEvent->cbFuncArg = cbFuncArg;

	return CELL_OK;
}

s32 sceNpMatching2GetRoomPasswordLocal()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2GetRoomDataExternalList()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2CreateJoinRoom()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2SignalingGetCtxOpt()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2GetLobbyInfoList()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2GetLobbyMemberIdListLocal()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2SendLobbyInvitation()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2ContextStop()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2SetLobbyMemberDataInternal()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2RegisterRoomMessageCallback()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2SignalingCancelPeerNetInfo()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2SignalingGetLocalNetInfo()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2SignalingGetPeerNetInfo()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpMatching2SignalingGetPeerNetInfoResult()
{
	UNIMPLEMENTED_FUNC(sceNp2);
	return CELL_OK;
}

s32 sceNpAuthOAuthInit()
{
	sceNp2.todo("sceNpAuthOAuthInit()");
	return CELL_OK;
}

s32 sceNpAuthOAuthTerm()
{
	sceNp2.todo("sceNpAuthOAuthTerm()");
	return CELL_OK;
}

s32 sceNpAuthCreateOAuthRequest()
{
	sceNp2.todo("sceNpAuthCreateOAuthRequest()");
	return CELL_OK;
}

s32 sceNpAuthDeleteOAuthRequest(SceNpAuthOAuthRequestId reqId)
{
	sceNp2.todo("sceNpAuthDeleteOAuthRequest(reqId=0x%x)", reqId);
	return CELL_OK;
}

s32 sceNpAuthAbortOAuthRequest(SceNpAuthOAuthRequestId reqId)
{
	sceNp2.todo("sceNpAuthAbortOAuthRequest(reqId=0x%x)", reqId);
	return CELL_OK;
}

s32 sceNpAuthGetAuthorizationCode(SceNpAuthOAuthRequestId reqId, vm::cptr<SceNpAuthGetAuthorizationCodeParameter> param, vm::ptr<SceNpAuthorizationCode> authCode, vm::ptr<s32> issuerId)
{
	sceNp2.todo("sceNpAuthGetAuthorizationCode(reqId=0x%x, param=*0x%x, authCode=*0x%x, issuerId=*0x%x)", reqId, param, authCode, issuerId);
	return CELL_OK;
}

DECLARE(ppu_module_manager::sceNp2)("sceNp2", []()
{
	REG_FUNC(sceNp2, sceNpMatching2DestroyContext);
	REG_FUNC(sceNp2, sceNpMatching2LeaveLobby);
	REG_FUNC(sceNp2, sceNpMatching2RegisterLobbyMessageCallback);
	REG_FUNC(sceNp2, sceNpMatching2GetWorldInfoList);
	REG_FUNC(sceNp2, sceNpMatching2RegisterLobbyEventCallback);
	REG_FUNC(sceNp2, sceNpMatching2GetLobbyMemberDataInternalList);
	REG_FUNC(sceNp2, sceNpMatching2SearchRoom);
	REG_FUNC(sceNp2, sceNpMatching2SignalingGetConnectionStatus);
	REG_FUNC(sceNp2, sceNpMatching2SetUserInfo);
	REG_FUNC(sceNp2, sceNpMatching2GetClanLobbyId);
	REG_FUNC(sceNp2, sceNpMatching2GetLobbyMemberDataInternal);
	REG_FUNC(sceNp2, sceNpMatching2ContextStart);
	REG_FUNC(sceNp2, sceNpMatching2CreateServerContext);
	REG_FUNC(sceNp2, sceNpMatching2GetMemoryInfo);
	REG_FUNC(sceNp2, sceNpMatching2LeaveRoom);
	REG_FUNC(sceNp2, sceNpMatching2SetRoomDataExternal);
	REG_FUNC(sceNp2, sceNpMatching2Term2);
	REG_FUNC(sceNp2, sceNpMatching2SignalingGetConnectionInfo);
	REG_FUNC(sceNp2, sceNpMatching2SendRoomMessage);
	REG_FUNC(sceNp2, sceNpMatching2JoinLobby);
	REG_FUNC(sceNp2, sceNpMatching2GetRoomMemberDataExternalList);
	REG_FUNC(sceNp2, sceNpMatching2AbortRequest);
	REG_FUNC(sceNp2, sceNpMatching2Term);
	REG_FUNC(sceNp2, sceNpMatching2GetServerInfo);
	REG_FUNC(sceNp2, sceNpMatching2GetEventData);
	REG_FUNC(sceNp2, sceNpMatching2GetRoomSlotInfoLocal);
	REG_FUNC(sceNp2, sceNpMatching2SendLobbyChatMessage);
	REG_FUNC(sceNp2, sceNpMatching2Init);
	REG_FUNC(sceNp2, sceNp2Init);
	REG_FUNC(sceNp2, sceNpMatching2AbortContextStart);
	REG_FUNC(sceNp2, sceNpMatching2GetRoomMemberIdListLocal);
	REG_FUNC(sceNp2, sceNpMatching2JoinRoom);
	REG_FUNC(sceNp2, sceNpMatching2GetRoomMemberDataInternalLocal);
	REG_FUNC(sceNp2, sceNpMatching2GetCbQueueInfo);
	REG_FUNC(sceNp2, sceNpMatching2KickoutRoomMember);
	REG_FUNC(sceNp2, sceNpMatching2ContextStartAsync);
	REG_FUNC(sceNp2, sceNpMatching2SetSignalingOptParam);
	REG_FUNC(sceNp2, sceNpMatching2RegisterContextCallback);
	REG_FUNC(sceNp2, sceNpMatching2SendRoomChatMessage);
	REG_FUNC(sceNp2, sceNpMatching2SetRoomDataInternal);
	REG_FUNC(sceNp2, sceNpMatching2GetRoomDataInternal);
	REG_FUNC(sceNp2, sceNpMatching2SignalingGetPingInfo);
	REG_FUNC(sceNp2, sceNpMatching2GetServerIdListLocal);
	REG_FUNC(sceNp2, sceNpUtilBuildCdnUrl);
	REG_FUNC(sceNp2, sceNpMatching2GrantRoomOwner);
	REG_FUNC(sceNp2, sceNpMatching2CreateContext);
	REG_FUNC(sceNp2, sceNpMatching2GetSignalingOptParamLocal);
	REG_FUNC(sceNp2, sceNpMatching2RegisterSignalingCallback);
	REG_FUNC(sceNp2, sceNpMatching2ClearEventData);
	REG_FUNC(sceNp2, sceNp2Term);
	REG_FUNC(sceNp2, sceNpMatching2GetUserInfoList);
	REG_FUNC(sceNp2, sceNpMatching2GetRoomMemberDataInternal);
	REG_FUNC(sceNp2, sceNpMatching2SetRoomMemberDataInternal);
	REG_FUNC(sceNp2, sceNpMatching2JoinProhibitiveRoom);
	REG_FUNC(sceNp2, sceNpMatching2SignalingSetCtxOpt);
	REG_FUNC(sceNp2, sceNpMatching2DeleteServerContext);
	REG_FUNC(sceNp2, sceNpMatching2SetDefaultRequestOptParam);
	REG_FUNC(sceNp2, sceNpMatching2RegisterRoomEventCallback);
	REG_FUNC(sceNp2, sceNpMatching2GetRoomPasswordLocal);
	REG_FUNC(sceNp2, sceNpMatching2GetRoomDataExternalList);
	REG_FUNC(sceNp2, sceNpMatching2CreateJoinRoom);
	REG_FUNC(sceNp2, sceNpMatching2SignalingGetCtxOpt);
	REG_FUNC(sceNp2, sceNpMatching2GetLobbyInfoList);
	REG_FUNC(sceNp2, sceNpMatching2GetLobbyMemberIdListLocal);
	REG_FUNC(sceNp2, sceNpMatching2SendLobbyInvitation);
	REG_FUNC(sceNp2, sceNpMatching2ContextStop);
	REG_FUNC(sceNp2, sceNpMatching2Init2);
	REG_FUNC(sceNp2, sceNpMatching2SetLobbyMemberDataInternal);
	REG_FUNC(sceNp2, sceNpMatching2RegisterRoomMessageCallback);
	REG_FUNC(sceNp2, sceNpMatching2SignalingCancelPeerNetInfo);
	REG_FUNC(sceNp2, sceNpMatching2SignalingGetLocalNetInfo);
	REG_FUNC(sceNp2, sceNpMatching2SignalingGetPeerNetInfo);
	REG_FUNC(sceNp2, sceNpMatching2SignalingGetPeerNetInfoResult);

	REG_FUNC(sceNp2, sceNpAuthOAuthInit);
	REG_FUNC(sceNp2, sceNpAuthOAuthTerm);
	REG_FUNC(sceNp2, sceNpAuthCreateOAuthRequest);
	REG_FUNC(sceNp2, sceNpAuthDeleteOAuthRequest);
	REG_FUNC(sceNp2, sceNpAuthAbortOAuthRequest);
	REG_FUNC(sceNp2, sceNpAuthGetAuthorizationCode);
});
