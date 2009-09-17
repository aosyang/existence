/**********************************************************************
 *<
	FILE: MaxExporter.cpp

	DESCRIPTION:	Appwizard generated plugin

	CREATED BY: 

	HISTORY: 

 *>	Copyright (c) 2003, All Rights Reserved.
 **********************************************************************/

#include "MaxExporter.h"

#include <fstream>

#define ExistanceMaxExporter_CLASS_ID	Class_ID(0x88fb4063, 0x7b7e0b6b)

class ExistanceMaxExporter : public SceneExport {
	public:
		
		static HWND hParams;
		
		int				ExtCount();					// Number of extensions supported
		const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
		const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
		const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
		const TCHAR *	AuthorName();				// ASCII Author name
		const TCHAR *	CopyrightMessage();			// ASCII Copyright message
		const TCHAR *	OtherMessage1();			// Other message #1
		const TCHAR *	OtherMessage2();			// Other message #2
		unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
		void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box

		BOOL SupportsOptions(int ext, DWORD options);
		int				DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);

		//Constructor/Destructor
		ExistanceMaxExporter();
		~ExistanceMaxExporter();		

};



class ExistanceMaxExporterClassDesc : public ClassDesc2 {
	public:
	int 			IsPublic() { return TRUE; }
	void *			Create(BOOL loading = FALSE) { return new ExistanceMaxExporter(); }
	const TCHAR *	ClassName() { return GetString(IDS_CLASS_NAME); }
	SClass_ID		SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
	Class_ID		ClassID() { return ExistanceMaxExporter_CLASS_ID; }
	const TCHAR* 	Category() { return GetString(IDS_CATEGORY); }

	const TCHAR*	InternalName() { return _T("ExistanceMaxExporter"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance() { return hInstance; }					// returns owning module handle
	

};

static ExistanceMaxExporterClassDesc ExistanceMaxExporterDesc;
ClassDesc2* GetExistanceMaxExporterDesc() { return &ExistanceMaxExporterDesc; }





INT_PTR CALLBACK ExistanceMaxExporterOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) {
	static ExistanceMaxExporter *imp = NULL;

	switch(message) {
		case WM_INITDIALOG:
			imp = (ExistanceMaxExporter *)lParam;
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return 1;
	}
	return 0;
}


//--- ExistanceMaxExporter -------------------------------------------------------
ExistanceMaxExporter::ExistanceMaxExporter()
{

}

ExistanceMaxExporter::~ExistanceMaxExporter() 
{

}

int ExistanceMaxExporter::ExtCount()
{
	//TODO: Returns the number of file name extensions supported by the plug-in.
	return 1;
}

const TCHAR *ExistanceMaxExporter::Ext(int n)
{		
	//TODO: Return the 'i-th' file name extension (i.e. "3DS").
	return _T("emd");
}

const TCHAR *ExistanceMaxExporter::LongDesc()
{
	//TODO: Return long ASCII description (i.e. "Targa 2.0 Image File")
	return _T("Existance Mesh File");
}
	
const TCHAR *ExistanceMaxExporter::ShortDesc() 
{			
	//TODO: Return short ASCII description (i.e. "Targa")
	return _T("E-Mesh");
}

const TCHAR *ExistanceMaxExporter::AuthorName()
{			
	//TODO: Return ASCII Author name
	return _T("Mwolf");
}

const TCHAR *ExistanceMaxExporter::CopyrightMessage() 
{	
	// Return ASCII Copyright message
	return _T("");
}

const TCHAR *ExistanceMaxExporter::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *ExistanceMaxExporter::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int ExistanceMaxExporter::Version()
{				
	//TODO: Return Version number * 100 (i.e. v3.01 = 301)
	return 100;
}

void ExistanceMaxExporter::ShowAbout(HWND hWnd)
{			
	// Optional
}

BOOL ExistanceMaxExporter::SupportsOptions(int ext, DWORD options)
{
	// TODO Decide which options to support.  Simply return
	// true for each option supported by each Extension 
	// the exporter supports.

	return TRUE;
}

int	ExistanceMaxExporter::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	//TODO: Implement the actual file Export here and 
	//		return TRUE If the file is exported properly

	//if(!suppressPrompts)
	//	DialogBoxParam(hInstance, 
	//			MAKEINTRESOURCE(IDD_PANEL), 
	//			GetActiveWindow(), 
	//			ExistanceMaxExporterOptionsDlgProc, (LPARAM)this);

	typedef int(*ExportFunc)(const TCHAR*, ExpInterface*, Interface*, BOOL, DWORD);

	TCHAR exeFullPath[_MAX_PATH];
	char szDrive[_MAX_DRIVE]={0}, szDir[_MAX_DIR]={0}, szFNAME[_MAX_FNAME]={0}, szExt[_MAX_EXT]={0};

	GetModuleFileName(hInstance, exeFullPath, sizeof(exeFullPath));
	_splitpath (exeFullPath, szDrive, szDir, NULL, NULL);

	char dllFullPath[256] = {0};
	strcat(dllFullPath, szDrive);
	strcat(dllFullPath, szDir);
	strcat(dllFullPath, "MaxExporter.dll");

	HINSTANCE hDLL;
	hDLL = LoadLibrary(dllFullPath);

	ExportFunc exportFunc = (ExportFunc)GetProcAddress(hDLL, "DoExport");

	if (!exportFunc)
		return FALSE;

	BOOL result = (*exportFunc)(name, ei, i, suppressPrompts, options);

	FreeLibrary(hDLL);

	return result;
}
