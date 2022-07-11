#pragma once

#include <attn/attn_config.hpp>
#include <common/interfaces.hpp>

#include <memory>

namespace attn
{

/**
 * @brief Load the attention handler as a gpio monitor
 *
 * Request the attention gpio for monitoring and attach the attention handler
 * as the gpio event handler.
 *
 * @param i_config     pointer to attention handler configuration object
 *
 * @return 0 == success
 */
int attnDaemon(Config* i_config, Interfaces& i_interfaces);

} // namespace attn
