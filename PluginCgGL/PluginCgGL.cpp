#include "CgGLProgram.h"
#include "EString.h"
#include "Debug.h"

#include <Cg/cg.h>
#include <Cg/cgGL.h>

CGcontext		g_CgContext;
CGprofile		g_CgVertexProfile,
				g_CgFragmentProfile;


void CheckCgError()
{
	CGerror error;
	const char* errorStr = cgGetLastErrorString(&error);

	if (error != CG_NO_ERROR)
	{
		if (error == CG_COMPILER_ERROR)
		{
			//errorStr.append(cgGetLastListing(m_CgContext));
			AssertFatal(0, "CheckCgError() : Error checking cg!");
		}
		//AssertFatal(0, errorStr.Data());
		AssertFatal(0, "CheckCgError() : Error checking cg!");
	}
}


extern "C" DLLEXPORT void CreateGpuPlugin()
{
	g_CgContext = cgCreateContext();
	CheckCgError();

	cgSetParameterSettingMode(g_CgContext, CG_DEFERRED_PARAMETER_SETTING);

	// 获得Vertex Profile
	g_CgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
	cgGLSetOptimalOptions(g_CgVertexProfile);
	CheckCgError();

	// 获得Fragment Profile
	g_CgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
	cgGLSetOptimalOptions(g_CgFragmentProfile);
	CheckCgError();

}

extern "C" DLLEXPORT void DestroyGpuPlugin()
{
    cgDestroyContext(g_CgContext);
}

extern "C" DLLEXPORT IGpuProgram* LoadGpuProgram(const String& filename, const String& entry, GpuProgramType type)
{
	CGprofile* profile;
	switch(type)
	{
	case GPU_VERTEX_PROGRAM:
		profile = &g_CgVertexProfile;
		break;
	case GPU_FRAGMENT_PROGRAM:
		profile = &g_CgFragmentProfile;
		break;
	default:
		AssertFatal(0, "LoadProgramFromFile() : Invalid program type!");
	};

	CgGLProgram* program = new CgGLProgram();
	program->m_CGProgram = cgCreateProgramFromFile(g_CgContext, CG_SOURCE, filename.Data(), *profile, entry.Data(), NULL);
	CheckCgError();
	cgGLLoadProgram(program->m_CGProgram);
	CheckCgError();
	
	return program;
}

bool	g_VPEnabled = false;
bool	g_FPEnabled = false;

extern "C" DLLEXPORT void BindGpuProgram(IGpuProgram* program, GpuProgramType type)
{
	cgGLBindProgram(static_cast<CgGLProgram*>(program)->m_CGProgram);
	CheckCgError();

	CGprofile* profile;
	switch(type)
	{
	case GPU_VERTEX_PROGRAM:
		profile = &g_CgVertexProfile;
		g_VPEnabled = true;
		break;
	case GPU_FRAGMENT_PROGRAM:
		profile = &g_CgFragmentProfile;
		g_FPEnabled = true;
		break;
	default:
		AssertFatal(0, "LoadProgramFromFile() : Invalid program type!");
	};

	cgGLEnableProfile(*profile);
	CheckCgError();
}

extern "C" DLLEXPORT void UnbindGpuProgram(GpuProgramType type)
{
	switch(type)
	{
	case GPU_VERTEX_PROGRAM:
		if (!g_VPEnabled) return;
		cgGLDisableProfile(g_CgVertexProfile);
		g_VPEnabled = false;
		break;
	case GPU_FRAGMENT_PROGRAM:
		if (!g_FPEnabled) return;
		cgGLDisableProfile(g_CgFragmentProfile);
		g_FPEnabled = false;
		break;
	default:
		AssertFatal(0, "LoadProgramFromFile() : Invalid program type!");
	};
}
