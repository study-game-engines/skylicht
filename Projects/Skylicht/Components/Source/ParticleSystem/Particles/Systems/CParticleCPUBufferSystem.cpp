/*
!@
MIT License

Copyright (c) 2020 Skylicht Technology CO., LTD

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

#include "pch.h"
#include "CParticleCPUBufferSystem.h"

#include "ParticleSystem/Particles/CParticle.h"
#include "ParticleSystem/Particles/CGroup.h"

#include "ParticleSystem/Particles/Renderers/CBillboardAdditiveRenderer.h"

namespace Skylicht
{
	namespace Particle
	{
		CParticleCPUBufferSystem::CParticleCPUBufferSystem()
		{

		}

		CParticleCPUBufferSystem::~CParticleCPUBufferSystem()
		{

		}

		void CParticleCPUBufferSystem::update(CParticle *particles, int num, CGroup *group, float dt)
		{
			CBillboardAdditiveRenderer *billboard = dynamic_cast<CBillboardAdditiveRenderer*>(group->getRenderer());
			IMeshBuffer *mb = group->getParticleBuffer()->getMeshBuffer();

			if (billboard != NULL)
			{
				billboard->updateParticleBuffer(mb, particles, num);
			}
		}
	}
}