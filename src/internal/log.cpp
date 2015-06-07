/*
 * freelan - An open, multi-platform software to establish peer-to-peer virtual
 * private networks.
 *
 * Copyright (C) 2010-2011 Julien KAUFFMANN <julien.kauffmann@freelan.org>
 *
 * This file is part of freelan.
 *
 * freelan is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * freelan is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 *
 * If you intend to use freelan in a commercial software, please
 * contact me : we may arrange this for a small fee or no fee at all,
 * depending on the nature of your project.
 */

#include "log.hpp"

namespace freelan {

namespace {
	static LogFunction log_function;

	static int on_log_callback(FreeLANLogLevel level, FreeLANTimestamp timestamp, const char* domain, const char* code, size_t payload_size, const struct FreeLANLogPayload* payload, const char* file, unsigned int line) {
		if (log_function) {
			static const boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
			const boost::posix_time::ptime ts = epoch + boost::posix_time::microseconds(static_cast<unsigned int>(timestamp * 1000000.0f));

			return log_function(static_cast<LogLevel>(level), ts, domain, code, payload_size, payload, file, line) ? 1 : 0;
		}

		return 0;
	}
}

void set_log_function(LogFunction function) {
	log_function = function;

	if (log_function) {
		::freelan_set_logging_callback(&on_log_callback);
	} else {
		::freelan_set_logging_callback(nullptr);
	}
}

}