#pragma once

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "noncopyable.h"

#include "renderer/render_impl/render_impl.h"
#include "renderer/shadersection.h"
#include "renderer/shader.h"
#include "renderer/vertexbuffer.h"
#include "renderer/indexbuffer.h"
#include "renderer/image.h"
#include "renderer/texture2d.h"
#include "renderer/renderer.h"
#include "renderer/icanvas.h"
#include "renderer/window.h"

#include "scene/scene.h"
#include "scene/entity.h"
