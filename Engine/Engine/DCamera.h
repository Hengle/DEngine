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

class DCamera;

struct DCameraNode
{
public:
	DCamera* camera;
	DCameraNode* next;
	DCameraNode* pre;
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
	/*设置fov*/
	void SetFieldOfView(float);
	/*设置近裁面*/
	void SetNear(float);
	/*设置远裁面*/
	void SetFar(float);
	void SetAspect(float);
	void SetOrthographic(bool);
	void SetOrthoSize(float);
	/*设置相机滤镜*/
	void SetFilter(ICameraFilter* filter);
	/*设置相机背景色*/
	void SetBackgroundColor(DColor&);
	void SetSkyBox(DMaterial*);
	/*设置相机视口*/
	void SetViewPort(DRect&);
	void ClearSkyBox();
	void ClearFilter();
	DRenderTexture* GetRenderTexture();
	void SetRenderTexture(DRenderTexture*);
	void SetReplaceShader(DShader* replacement);
	void ResetReplaceShader();

	static void GetCurrentCamera(DCamera** cam);

protected:
	virtual bool OnInit();
	virtual void OnDestroy();
	virtual void OnUpdate();
	virtual void OnFixedUpdate();
	virtual void OnRender();

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
	/*标记投影矩阵是否发生变化*/
	bool m_isProjectionChanged;
	ICameraFilter* m_filter;
	DRenderTexture* m_renderTexture;
	DColor m_backgroundColor;
	DMaterial* m_skyBoxMaterial;
	DRect m_viewPort;
	DShader* m_replacementShader;
	/*保存当前的相机节点*/
	DCameraNode* m_node;
};

static DCamera* sCurrent;

