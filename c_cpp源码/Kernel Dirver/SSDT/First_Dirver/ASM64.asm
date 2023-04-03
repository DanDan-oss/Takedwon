;#pragma once
.data	
	SeDebugPrivilege
.code		
	GetseDebugPrivilege proc	
		mov     rax, cs:SeDebugPrivilege
		ret
	GetseDebugPrivilege endp	
end	
