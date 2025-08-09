Clay_RenderCommandArray createLayout() {
    Clay_BeginLayout();

    CLAY({
        .id = CLAY_ID("OuterBody"),
        .layout = {
            .sizing = {
                .width = CLAY_SIZING_GROW(0),
                .height = CLAY_SIZING_GROW(0),
            },
            .childAlignment = {
                .x = CLAY_ALIGN_Y_CENTER,
            }
        }
    }) {
        CLAY({
            .id = CLAY_ID("Header"),
            .backgroundColor = (Clay_Color){30, 100, 255, 255},
        }) {
            CLAY_TEXT(
                    CLAY_STRING("69"),
                    CLAY_TEXT_CONFIG({
                        .textColor = (Clay_Color){255, 189, 189, 255}
                    })
            );
        }
    }

    return Clay_EndLayout();
}
