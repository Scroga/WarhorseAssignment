#pragma once
#include <string>
#include <tuple>

#include "utils/components.h"
#include "utils/game_clock.h"

class EntityManager;

enum class MovementDir { RIGHT, LEFT };

using ComponentsTuple = std::tuple<
	CTransform,
	CCollision,
	CRenderer,
	CLifeSpan,
	CHealth,
	CScore>;

template<typename T>
using EnableIfComponent = std::enable_if_t<std::is_base_of<Component, T>::value>;

class Entity {
protected:
	friend class EntityManager;

	bool is_alive_ = true;
	std::string tag_ = "none";
	size_t id_ = 0;
	ComponentsTuple components_;

	// constructor is protected so we can never create
	// entites outside the EntityManager which has friend access
	Entity(const size_t id, const std::string& tag) : id_(id), tag_(tag) {};
	Entity(const Entity&) = default;
	Entity& operator=(const Entity&) = default;
	Entity(Entity&&) noexcept = default;
	Entity& operator=(Entity&&) noexcept = default;
public:
	virtual ~Entity() = default;

	virtual void update(const GameClock& clock) {}

	std::string tag() const { return tag_; }
	size_t id() const { return id_; }
	bool is_alive() const { return is_alive_; }
	void destroy() { is_alive_ = false; }

	template <typename T, typename = EnableIfComponent<T>>
	T& get_component() {
		return std::get<T>(components_);
	}

	template <typename T, typename = EnableIfComponent<T>>
	const T& get_component() const {
		return std::get<T>(components_);
	}

	template <typename T, typename = EnableIfComponent<T>>
	void remove_component() {
		get_component<T>() = T();
	}

	template <typename T, typename = EnableIfComponent<T>>
	bool has_component() const {
		return get_component<T>().has;
	}

	template <typename T, typename... TArgs, typename = EnableIfComponent<T>>
	T& add_component(TArgs&&... args) {
		auto& component = get_component<T>();
		component = T(std::forward<TArgs>(args)...);
		component.has = true;
		return component;
	}
};