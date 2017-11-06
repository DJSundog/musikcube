package io.casey.musikcube.remote.data

import io.reactivex.Observable

interface IDataProvider {
    enum class State { Disconnected, Connecting, Connected }

    fun attach()
    fun detach()
    fun destroy()

    fun observeState(): Observable<Pair<State, State>>
    fun observePlayQueue(): Observable<Unit>
    fun observeAuthFailure(): Observable<Unit>

    fun getAlbums(filter: String = ""): Observable<List<IAlbum>>
    fun getAlbumsForCategory(categoryType: String, categoryId: Long, filter: String = ""): Observable<List<IAlbum>>

    fun getTrackCount(filter: String = ""): Observable<Int>
    fun getTracks(filter: String = ""): Observable<List<ITrack>>
    fun getTracks(limit: Int, offset: Int, filter: String = ""): Observable<List<ITrack>>

    fun getTrackCountByCategory(category: String, id: Long, filter: String = ""): Observable<Int>
    fun getTracksByCategory(category: String, id: Long, filter: String = ""): Observable<List<ITrack>>
    fun getTracksByCategory(category: String, id: Long, limit: Int, offset: Int, filter: String = ""): Observable<List<ITrack>>

    fun getPlayQueueTracksCount(filter: String = ""): Observable<Int>
    fun getPlayQueueTracks(filter: String = ""): Observable<List<ITrack>>
    fun getPlayQueueTracks(limit: Int, offset: Int, filter: String = ""): Observable<List<ITrack>>

    fun getCategoryValues(type: String, filter: String = ""): Observable<List<ICategoryValue>>

    fun appendToPlaylist(playlistId: Long, categoryType: String = "", categoryId: Long = -1, filter: String = "", offset: Long = -1): Observable<Boolean>

    val state: State
}