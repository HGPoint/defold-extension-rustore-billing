#pragma once

#include <queue>
#include <mutex>

namespace RuStoreSDK
{
	struct QueueCallbackItem
	{
		const std::string channel;
		const std::string value;

		QueueCallbackItem(const std::string& channel, const std::string& value)
		: channel(channel), value(value) {
		}
	};

	class QueueCallbackManager
	{
		private:
		std::mutex _mutex;
		std::queue<std::shared_ptr<QueueCallbackItem>> _queue;

		public:
		void PushQueueCallback(std::shared_ptr<QueueCallbackItem> item);
		std::queue<std::shared_ptr<QueueCallbackItem>> GetExexuteQueueCallback(int max = -1);
		
		static QueueCallbackManager* Instance();
	};
}
