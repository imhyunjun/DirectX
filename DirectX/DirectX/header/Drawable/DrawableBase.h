#pragma once
#include "Drawable.h"
#include "../Bindable/IndexBuffer.h"

template <class T>
class DrawableBase : public Drawable
{
protected:
	static bool IsStaticInitialized()
	{
		return !staticBinds.empty();
	}

	static void AddStaticBind(std::unique_ptr<Bindable> _bind) noexcept(!IS_DEBUG)
	{
		assert("Must use AddIndexBuffer to bind index buffer" && typeid(*_bind) != typeid(IndexBuffer));
		staticBinds.push_back(std::move(_bind));

	}

	void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> _ibuf) noexcept(!IS_DEBUG)
	{
		assert(pIndexBuffer == nullptr);
		pIndexBuffer = _ibuf.get();
		staticBinds.push_back(std::move(_ibuf));
	}

	//2개이상의 static instance를 만들때 indexbuffer가 nullpt인 현상 해결
	void SetIndexFromStatic() noexcept(!IS_DEBUG)
	{
		assert("Attempting to add Index Buffer a second time" && pIndexBuffer == nullptr);

		for (const auto& b : staticBinds)
		{
			if (const auto p = dynamic_cast<IndexBuffer*>(b.get()))
			{
				pIndexBuffer = p;
				return;
			}
		}

		assert("Failed to Find index buffer in static binds" && pIndexBuffer == nullptr);
	}

private:
	const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept
	{
		return staticBinds;
	}

private:
	static std::vector<std::unique_ptr<Bindable>> staticBinds;
};

template<class T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds;