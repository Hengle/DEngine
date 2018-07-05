#pragma once
#include "DSceneObject.h"
#include "DTexture.h"
#include "DMaterial.h"
#include "DEngineDefine.h"

/*相机滤镜接口*/
interface ICameraFilter
{
public:
	/*渲染滤镜*/
	virtual void Render(DRenderTexture* screenTexture /*相机RenderTexture*/) = 0;
	/*滤镜释放*/
	virtual void Release() = 0;
};

/*相机类*/
class DCamera : public DSceneObject
{
public:
	DCamera();
	~DCamera();
	//void BeginRender();
	//void EndRender();
	/*渲染滤镜*/
	void RenderFilter();
	/*渲染当前相机*/
	void Render();
	/*使用指定shader渲染*/
	void Render(DShader*);
	virtual void Init();
	virtual void Destroy();
	/*返回当前相机的视空间矩阵*/
	void GetViewMatrix(DMatrix4x4& out) const;
	/*返回当前相机的投影空间矩阵*/
	void GetProjection(DMatrix4x4& out) const;
	/*返回当前相机背景颜色*/
	void GetBackgroundColor(DColor&) const;
	/*返回当前相机FOV*/
	float GetFieldOfView() const;
	/*返回当前相机近裁面*/
	float GetNear() const;
	/*返回当前相机远裁面*/
	float GetFar() const;
	/*返回当前相机Aspect*/
	float GetAspect() const;
	float GetOrthoSize() const;
	/*返回当前相机视口区域*/
	void GetViewPort(DRect&) const;
	/*获取相机滤镜*/
	ICameraFilter* GetFilter() const;
	DMaterial* GetSkuBox() const;
	/*是否为正交投影*/
	bool IsOrthographic() const;
	void SetFieldOfView(float);
	void SetNear(float);
	void SetFar(float);
	void SetAspect(float);
	void SetOrthographic(bool);
	void SetOrthoSize(float);
	/*设置相机滤镜*/
	void SetFilter(ICameraFilter* filter);
	void SetBackgroundColor(DColor&);
	void SetSkyBox(DMaterial*);
	void SetViewPort(DRect&);
	void ClearSkyBox();
	void ClearFilter();
	DRenderTexture* GetRenderTexture();
	void SetRenderTexture(DRenderTexture*);

	static void GetCurrentCamera(DCamera** cam);

private:
	void BeginRender();
	void EndRender();

private:
	float m_fieldOfView;
	float m_near;
	float m_far;
	float m_aspect;
	float m_orthoSize;
	bool m_ortho;
	DMatrix4x4 m_viewMatrix;
	DMatrix4x4 m_projection;
	bool m_isProjectionChanged;
	ICameraFilter* m_filter;
	DRenderTexture* m_renderTexture;
	DColor m_backgroundColor;
	DMaterial* m_skyBoxMaterial;
	DRect m_viewPort;
};

static DCamera* sCurrent;

