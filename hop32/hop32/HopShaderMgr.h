#ifndef _HOP_SHADER_MGR
#define _HOP_SHADER_MGR

class HopShaderMgr
{

public:

	void load();
	void useHopProgram();
	int getPositionAttrib();
	int getTexCoordAttrib();
	int getTextureUniform();

	static HopShaderMgr *getInstance();

private:

	HopShaderMgr();
	~HopShaderMgr();

	unsigned int m_hopProgram;
	static HopShaderMgr s_instance;
};

#endif