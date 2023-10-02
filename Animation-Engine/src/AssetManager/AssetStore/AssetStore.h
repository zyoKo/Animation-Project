#pragma once

#include "Core/Logger/Log.h"

namespace Animator
{
	template <typename T>
	class AssetStore
	{
	public:
		void AddToStorage(const std::string& assetName, const std::shared_ptr<T> assetData, bool forceReplace = false);

		std::shared_ptr<T> RetrieveFromStorage(const std::string& assetName);

		void Clear();

	private:
		std::unordered_map<std::string, std::shared_ptr<T>> assetStorage;
	};
}

#include "AssetStore.inl"