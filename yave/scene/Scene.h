/*******************************
Copyright (C) 2013-2016 gregoire ANGERAND

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
**********************************/
#ifndef YAVE_SCENE_SCENE_H
#define YAVE_SCENE_SCENE_H

#include <yave/yave.h>
#include <yave/objects/StaticMesh.h>

namespace yave {

class Scene : NonCopyable {

	public:
		Scene(core::Vector<StaticMesh>&& meshes);

		const core::Vector<StaticMesh>& static_meshes() const;

	private:
		core::Vector<StaticMesh> _statics;
};


}

#endif // YAVE_SCENE_SCENE_H