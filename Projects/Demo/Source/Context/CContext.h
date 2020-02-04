#pragma once

#include "Utils/CGameSingleton.h"

#include "Scene/CScene.h"
#include "Lighting/CDirectionalLight.h"

#include "RenderPipeline/CForwardRP.h"
#include "RenderPipeline/CDeferredRP.h"
#include "RenderPipeline/CShadowMapRP.h"

class CContext : public CGameSingleton<CContext>
{
protected:
	CScene *m_scene;
	
	CZone *m_zone;
	CCamera *m_camera;
	CDirectionalLight *m_directionalLight;

	CBaseRP *m_beginRP;
	CBaseRP	*m_rendering;
	CShadowMapRP *m_shadowMapRendering;

public:
	CContext();

	virtual ~CContext();

	CScene* initScene();

	CBaseRP* initRenderPipeline(int w, int h);
	
	inline CScene* getScene()
	{
		return m_scene;
	}

	inline CShadowMapRP* getShadowMapRenderPipeline()
	{
		return m_shadowMapRendering;
	}

	inline CBaseRP* getRenderPipeline()
	{
		return m_beginRP;
	}

	inline void setActiveZone(CZone *zone)
	{
		m_zone = zone;
	}

	inline CZone* getActiveZone()
	{
		return m_zone;
	}

	void updateDirectionLight();

	void setDirectionalLight(CDirectionalLight *light);

	inline CDirectionalLight* getDirectionalLight()
	{
		return m_directionalLight;
	}

	inline void setActiveCamera(CCamera *camera)
	{
		m_camera = camera;
	}

	inline CCamera* getActiveCamera()
	{
		return m_camera;
	}

	void releaseScene();

	void releaseRenderPipeline();
};