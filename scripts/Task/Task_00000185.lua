--����Ľ�������
function Task_Accept_00000185()
	local player = GetPlayer();
	if player:GetLev() < 90 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(185) or task:HasCompletedTask(185) or task:HasSubmitedTask(185) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(184) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(184) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(184) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000185()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 90 then
		return false;
	end
	if task:HasAcceptedTask(185) or task:HasCompletedTask(185) or task:HasSubmitedTask(185) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(184) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(184) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(184) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000185()
	if GetPlayer():GetTaskMgr():HasCompletedTask(185) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000185(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(185) == npcId and Task_Accept_00000185 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 185
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "异兽毕方";
	elseif task:GetTaskSubmitNpc(185) == npcId then
		if Task_Submit_00000185() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 185
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "异兽毕方";
		elseif task:HasAcceptedTask(185) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 185
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "异兽毕方";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000185_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "昆仑乃是上古神人之地，有不少异兽残存，他们修炼多年，道行高深，少侠可以去拿他们练练手。瑶池那里就有一些毕方聚集，少侠去试试吧。";
	action.m_ActionMsg = "我去会会这些上古异兽。";
	return action;
end

function Task_00000185_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "少侠真是修为深厚，天赋异禀啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000185_step_table = {
		[1] = Task_00000185_step_01,
		[10] = Task_00000185_step_10,
		};

function Task_00000185_step(step)
	if Task_00000185_step_table[step] ~= nil then
		return Task_00000185_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000185_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000185() then
		return false;
	end
	if not task:AcceptTask(185) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000185_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(185) then
		return false;
	end


	player:AddExp(150000);
	return true;
end

--��������
function Task_00000185_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(185);
end