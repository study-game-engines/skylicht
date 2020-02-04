/*
!@
MIT License

Copyright (c) 2019 Skylicht Technology CO., LTD

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This file is part of the "Skylicht Engine".
https://github.com/skylicht-lab/skylicht-engine
!#
*/

#pragma once

#include "Shader/CShaderParams.h"
#include "Shader/CShader.h"

#include "GameObject/CGameObject.h"

namespace Skylicht
{
	class CMaterial
	{
	public:
		struct SUniformTexture
		{
			std::string Name;
			std::string Path;
			ITexture* Texture;

			SUniformTexture()
			{
				Texture = NULL;
			}

			SUniformTexture* clone()
			{
				SUniformTexture *c = new SUniformTexture();
				c->Name = Name;
				c->Path = Path;
				c->Texture = Texture;
				return c;
			}
		};

		struct SUniformValue
		{
			std::string Name;
			float FloatValue[4];
			int FloatSize;
			bool ShaderDefaultValue;

			SUniformValue()
			{
				ShaderDefaultValue = false;
				FloatSize = 0;
				memset(FloatValue, 0, 4 * sizeof(float));
			}

			SUniformValue* clone()
			{
				SUniformValue *c = new SUniformValue();
				memcpy(c->FloatValue, FloatValue, sizeof(float) * 4);
				c->Name = Name;
				c->FloatSize = FloatSize;
				c->ShaderDefaultValue = ShaderDefaultValue;
				return c;
			}
		};

		struct SExtraParams
		{
			std::string ShaderPath;
			std::vector<SUniformValue*> UniformParams;
			std::vector<SUniformTexture*> UniformTextures;
		};

	private:
		CShaderParams m_shaderParams;

		std::string m_materialName;
		std::string m_shaderPath;

		std::vector<SUniformValue*> m_uniformParams;
		std::vector<SUniformTexture*> m_uniformTextures;

		std::vector<IMeshBuffer*> m_meshBuffers;

		std::vector<SExtraParams*> m_extras;

		ITexture *m_resourceTexture[MATERIAL_MAX_TEXTURES];
		ITexture *m_textures[MATERIAL_MAX_TEXTURES];
		ITexture *m_overrideTextures[CShader::ResourceCount];

		video::E_COMPARISON_FUNC m_zBuffer;
		bool m_zWriteEnable;
		bool m_backfaceCulling;
		bool m_frontfaceCulling;
		bool m_doubleSided;
		bool m_deferred;
		bool m_castShadow;
		bool m_manualInitMaterial;
		int m_shadowMapTextureSlot;

		CGameObject* m_owner;
		CShader* m_shader;

	public:
		CMaterial(const char *name, const char *shaderPath);

		virtual ~CMaterial();

		inline void setOwner(CGameObject *obj)
		{
			m_owner = obj;
		}

		inline const char* getName()
		{
			return m_materialName.c_str();
		}

		inline const char* getShaderPath()
		{
			return m_shaderPath.c_str();
		}

		inline bool isDeferred()
		{
			return m_deferred;
		}

		CMaterial* clone(CGameObject *gameObject);

		void deleteAllParams();
		void deleteExtramParams();

		void setUniform(const char *name, float f);
		void setUniform2(const char *name, float *f);
		void setUniform3(const char *name, float *f);
		void setUniform4(const char *name, float *f);

		const char *getUniformTextureName(int slot);
		void setUniformTexture(const char *name, const char *path, bool loadTexture = true);
		void setUniformTexture(const char *name, const char *path, std::vector<std::string>& folder, bool loadTexture = true);
		void setUniformTexture(const char *name, ITexture *texture);

		SExtraParams *newExtra(const char *shaderPath);
		void setExtraUniformTexture(SExtraParams *e, const char *name, const char *path);
		void setExtraUniform(SExtraParams *e, const char *name, float *f, int floatSize);

		inline std::vector<SUniformValue*>& getUniformParams()
		{
			return m_uniformParams;
		}

		inline std::vector<SUniformTexture*>& getUniformTexture()
		{
			return m_uniformTextures;
		}

		inline std::vector<SExtraParams*>& getExtraParams()
		{
			return m_extras;
		}

		SUniformValue* getUniform(const char *name);

		SUniformTexture* getUniformTexture(const char *name);

		SUniformValue* getExtraUniform(SExtraParams *e, const char *name);

		SUniformTexture* getExtraUniformTexture(SExtraParams *e, const char *name);

		CShaderParams& getShaderParams()
		{
			return m_shaderParams;
		}

		bool haveUniform(const char *name);

		void initMaterial();

		void setTexture(ITexture** textures, int num);

		void setProperty(const std::string& name, const std::string& value);

		void loadDefaultTexture();

		void loadRuntimeTexture();

		void loadUniformTexture();

		void buildDoubleSidedMesh();

		void addAffectMesh(IMeshBuffer *mesh);

		void removeAffectMesh(IMeshBuffer *mesh);

		void clearAllAffectMesh();

		void setOverrideResource(ITexture *texture, CShader::EResourceType type)
		{
			m_overrideTextures[type] = texture;
		}

	public:

		void changeShader(CShader *shader);

		void changeShader(const char *path);

		bool autoDetectLoadTexture();

		void applyMaterial();

		void applyMaterial(SMaterial& mat);

	protected:

		void initDefaultValue();

		void updateTexture(SMaterial& mat);

		void updateShaderTexture();

		void updateShaderParams();

		void setDefaultValue(SUniformValue *v, SUniform* u);

		SUniformValue *newUniform(const char *name, int floatSize);

		SUniformTexture *newUniformTexture(const char *name);

		SUniformTexture *newExtraUniformTexture(SExtraParams *e, const char *name);

		SUniformValue *newExtraUniform(SExtraParams *e, const char *name, int floatSize);

		void addShaderUI(CShader::SUniformUI* ui);

		SExtraParams* getExtraParams(const char *shaderPath);

		void saveExtraParams();

		void reloadExtraParams(const char *shaderPath);

		SUniformTexture *findExtraTexture(const char *name);

		SUniformValue *findExtraParam(const char *name, int floatSize);
	};

	typedef std::vector<CMaterial*> ArrayMaterial;
}