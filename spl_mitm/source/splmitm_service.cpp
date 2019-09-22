/*
 * Copyright (c) 2018 Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <switch.h>
#include "debug.hpp"

#include "splmitm_service.hpp"

Result SplMitMService::GenerateAesKek(Out<AesKey> out, GenerateAesKekParams p) {
    LogFormat("SplMitMService::GenerateAesKek generation: %" PRIu32 ", option: %" PRIu32 ", key_source", p.generation, p.option);
    LogHex(&p.key_source, sizeof(p.key_source));

    return ResultAtmosphereMitmShouldForwardToSession;
}
Result SplMitMService::GenerateAesKey(Out<AesKey> out, AesKey access_key, AesKey key_source) {
    LogFormat("SplMitMService::GenerateAesKey access_key key_source");
    LogHex(&access_key, sizeof(access_key));
    LogHex(&key_source, sizeof(key_source));

    return ResultAtmosphereMitmShouldForwardToSession;
}
