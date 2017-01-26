//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2007-2016 musikcube team
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the author nor the names of other contributors may
//      be used to endorse or promote products derived from this software
//      without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////////

#include "SavePlaylistQuery.h"

#include <core/db/ScopedTransaction.h>
#include <core/db/Statement.h>

using namespace musik::core;
using namespace musik::core::query;
using namespace musik::core::db;
using namespace musik::glue;

static std::string CREATE_PLAYLIST_QUERY =
    "INSERT INTO playlists (name) VALUES (?);";

static std::string INSERT_PLAYLIST_TRACK_QUERY =
    "INSERT INTO playlist_tracks (track_id, playlist_id, sort_order) VALUES (?, ?, ?);";

SavePlaylistQuery::SavePlaylistQuery(
    const std::string& playlistName,
    std::shared_ptr<musik::core::TrackList> tracks)
{
    this->playlistName = playlistName;
    this->tracks = tracks;
}

SavePlaylistQuery::~SavePlaylistQuery() {

}

bool SavePlaylistQuery::OnRun(musik::core::db::Connection &db) {
    ScopedTransaction transaction(db);

    /* create playlist */
    Statement createPlaylist(CREATE_PLAYLIST_QUERY.c_str(), db);
    createPlaylist.BindText(0, this->playlistName);

    if (createPlaylist.Step() == db::Error) {
        transaction.Cancel();
        return false;
    }

    DBID playlistId = db.LastInsertedId();

    /* add tracks to playlist */
    Statement insertTrack(INSERT_PLAYLIST_TRACK_QUERY.c_str(), db);
    TrackList& tracks = *this->tracks;
    TrackPtr track;
    for (size_t i = 0; i < tracks.Count(); i++) {
        track = tracks.Get(i);
        insertTrack.Reset();
        insertTrack.BindInt(0, track->Id());
        insertTrack.BindInt(1, playlistId);
        insertTrack.BindInt(2, (int) i);

        if (insertTrack.Step() == db::Error) {
            transaction.Cancel();
            return false;
        }
    }

    return true;
}