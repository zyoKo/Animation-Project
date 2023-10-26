#pragma once

#include "AssetStore.h"

namespace AnimationEngine
{
	template <typename T>
	void AssetStore<T>::AddToStorage(const std::string& assetName, const std::shared_ptr<T> assetData, bool forceReplace)
	{
		if (!assetStorage.contains(assetName) || forceReplace)
		{
			LOG_INFO("Successfully added asset[{0}] to Asset Storage!", assetName);
			assetStorage[assetName] = assetData;
			return;
		}

		LOG_INFO("Asset[{0}] is already present in the storage!", assetName);
	}

	template <typename T>
	std::shared_ptr<T> AssetStore<T>::RetrieveFromStorage(const std::string& assetName)
	{
		if (!assetStorage.contains(assetName))
		{
			LOG_WARN("Failed to retrive asset[{0}] from the Asset Storage!", assetName);
			return nullptr;
		}

		LOG_INFO("Successfully retrived asset[{0}] from Asset Storage!", assetName);
		return assetStorage.at(assetName);
	}

	template <typename T>
	void AssetStore<T>::Clear()
	{
		assetStorage.clear();
	}
}
