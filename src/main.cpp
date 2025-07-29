#include "config.h"

namespace Hook
{
	struct Hook_POVSwitch
	{
		/*
			Player camera states:
		    	FirstPerson = 0,
				ThirdPersonClose = 1,
				ThirdPersonFar = 2,
				Customization = 3,
				EVPlayerPOVType_MAX = 4
		*/

		// Middle mouse click POV handler
		// Default behaviour is to cycle the POV 0->1->2 then back to 0
		static void OnSwitchPOVFunc(char* playerChar, int targetPOV, bool setToNewDefaultState)
		{
			bool ignoreClose = Remember3rdPersonCam::Config::GetSingleton()->IgnoreClose();
			bool ignoreFar = Remember3rdPersonCam::Config::GetSingleton()->IgnoreFar();

			int newTargetPOV = targetPOV;

			// Only first person POV is desired
			if (ignoreClose && ignoreFar)
			{
				newTargetPOV = 0;
			}
			else
			{
				switch (targetPOV)
				{
					case 0:
						// Save current 3rd person POV 1 or 2
						if (ignoreFar)
						{
							last3rdPersonPOV = 1;
						}					
						else
						{
							last3rdPersonPOV = 2;	
						}

						break;

					case 1:
						if (ignoreClose) // Close cam is being ignored
						{
							newTargetPOV = 2;
						}
						else if (last3rdPersonPOV > -1) // Last 3rd person POV was saved, then force switch to that instead
						{
							// Force switch to previous 3rd person POV
							newTargetPOV = last3rdPersonPOV;
						}						

						break;

					case 2:
						if (ignoreClose) // Close cam is being ignored
						{
							// Save current 3rd person cam POV 2
							last3rdPersonPOV = 2;
						}
						else if (ignoreFar) // Far cam is being ignored
						{
							// Force switch to POV 0
							newTargetPOV = 0; 
						}
						else
						{
							// Save current 3rd person POV 1
							last3rdPersonPOV = 1;

							// Force switch to POV 0
							newTargetPOV = 0;					
						}					
						
						break;
					
					default:
						REX::INFO("Unknown targetPOV");
						break;
				}
			}
		
			// Call original function
			OnSwitchPOVFuncHook(playerChar, newTargetPOV, setToNewDefaultState);
		}

		static inline REL::Hook OnSwitchPOVFuncHook{ REL::ID(307859), 0x7C, OnSwitchPOVFunc };

		inline static std::atomic<std::int32_t> last3rdPersonPOV{ -1 };
	};	
}

OBSE_PLUGIN_LOAD(const OBSE::LoadInterface* a_obse)
{
	OBSE::Init(a_obse, { .trampoline = true, .trampolineSize = 16 });

	Remember3rdPersonCam::Config::GetSingleton();

	return true;
}