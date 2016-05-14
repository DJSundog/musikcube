#pragma once

#include "LayoutBase.h"
#include "CategoryListView.h"
#include "TrackListView.h"

#include <core/library/ILibrary.h>

using musik::core::LibraryPtr;

class LibraryLayout : public LayoutBase {
    public:
        LibraryLayout(LibraryPtr library);
        ~LibraryLayout(); /* not virtual */

        virtual void Layout();
        virtual void OnIdle();

    private:
        std::shared_ptr<CategoryListView> albumList;
        std::shared_ptr<TrackListView> trackList;
};