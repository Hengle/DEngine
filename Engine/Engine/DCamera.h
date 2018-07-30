#pragma once
#include "DSceneObject.h"
#include "DTexture.h"
#include "DMaterial.h"
#include "DEngineDefine.h"

#define D_LAYERMASK_NOTHING 0U
#define D_LAYERMASK_ALL     0xFFFFFFFFU
#define D_LAYERMASK_DEFAULT 1U

enum DClearFlags
{
	DClearFlags_SkyBox,
	DClearFlags_Color,
	DClearFlags_Depth,
	DClearFlags_DontClear,
};

/*相机额外渲染贴图类型*/
enum DCameraAdditionalTextureType
{
	/*不额外渲染*/
	DCameraAdditional_None = 0U,
	/*额外渲染屏幕纹理*/
	DCameraAdditional_Screen = 1U,
	/*额外渲染屏幕深度*/
	DCameraAdditional_Depth = 2U,
};

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
	void Render();
	
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
	int GetSortOrder() const;
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
	void SetSortOrder(int);
	void ClearSkyBox();
	void ClearFilter();
	DRenderTexture* GetRenderTexture();
	void SetRenderTexture(DRenderTexture*);
	void SetReplaceShader(DShader* replacement);
	void ResetReplaceShader();
	void SetClearFlags(DClearFlags);
	DClearFlags GetClearFlags();

	void SetLayerMask(DLAYER);
	DLAYER GetLayerMask();
	void AddLayer(DLAYER);
	void RemoveLayer(DLAYER);
	bool IsLayerVisible(DLAYER);

	static void GetCurrentCamera(DCamera** cam);

protected:
	virtual void OnPreRender() {};
	virtual void OnPostRender() {};
	virtual bool OnInit();
	virtual void OnDestroy();
	virtual void OnUpdate();
	virtual void OnFixedUpdate();

private:
	/*渲染滤镜*/
	void RenderFilter();
	void ForwardMoveCameraNode();
	void BackwardMoveCameraNode();
	void BeginRender();
	void EndRender();
	void RefreshCameraDirParam();

protected:
	int m_sortOrder;

	DLAYER m_layerMask;
	DClearFlags m_clearFlags;

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
	/*当前相机额外渲染纹理类型*/
};

static DCamera* sCurrent;

