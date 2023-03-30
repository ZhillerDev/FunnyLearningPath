### 此次分析代码

`components/Navbar.jsx` 代码清单

```jsx
import React, { useEffect, useState } from "react";
import { Link } from "react-router-dom";

import { styles } from "../styles";
import { navLinks } from "../constants";
import { logo, menu, close } from "../assets";

const Navbar = () => {
	const [active, setActive] = useState("");
	const [toggle, setToggle] = useState(false);
	const [scrolled, setScrolled] = useState(false);

	useEffect(() => {
		const handleScroll = () => {
			const scrollTop = window.scrollY;
			if (scrollTop > 100) {
				setScrolled(true);
			} else {
				setScrolled(false);
			}
		};

		window.addEventListener("scroll", handleScroll);

		return () => window.removeEventListener("scroll", handleScroll);
	}, []);

	return (
		<nav
			className={`${
				styles.paddingX
			} w-full flex items-center py-5 fixed top-0 z-20 ${
				scrolled ? "bg-primary" : "bg-transparent"
			}`}
		>
			<div className="w-full flex justify-between items-center max-w-7xl mx-auto">
				<Link
					to="/"
					className="flex items-center gap-2"
					onClick={() => {
						setActive("");
						window.scrollTo(0, 0);
					}}
				>
					<img
						src={logo}
						alt="logo"
						className="w-9 h-9 object-contain"
					/>
					<p className="text-white text-[18px] font-bold cursor-pointer flex ">
						Adrian &nbsp;
						<span className="sm:block hidden"> | JavaScript Mastery</span>
					</p>
				</Link>

				<ul className="list-none hidden sm:flex flex-row gap-10">
					{navLinks.map((nav) => (
						<li
							key={nav.id}
							className={`${
								active === nav.title ? "text-white" : "text-secondary"
							} hover:text-white text-[18px] font-medium cursor-pointer`}
							onClick={() => setActive(nav.title)}
						>
							<a href={`#${nav.id}`}>{nav.title}</a>
						</li>
					))}
				</ul>

				<div className="sm:hidden flex flex-1 justify-end items-center">
					<img
						src={toggle ? close : menu}
						alt="menu"
						className="w-[28px] h-[28px] object-contain"
						onClick={() => setToggle(!toggle)}
					/>

					<div
						className={`${
							!toggle ? "hidden" : "flex"
						} p-6 black-gradient absolute top-20 right-0 mx-4 my-2 min-w-[140px] z-10 rounded-xl`}
					>
						<ul className="list-none flex justify-end items-start flex-1 flex-col gap-4">
							{navLinks.map((nav) => (
								<li
									key={nav.id}
									className={`font-poppins font-medium cursor-pointer text-[16px] ${
										active === nav.title ? "text-white" : "text-secondary"
									}`}
									onClick={() => {
										setToggle(!toggle);
										setActive(nav.title);
									}}
								>
									<a href={`#${nav.id}`}>{nav.title}</a>
								</li>
							))}
						</ul>
					</div>
				</div>
			</div>
		</nav>
	);
};

export default Navbar;
```

<br>

### 滚动检测

使用 `useEffect` 副作用实时检测当前滚动状态，如果滚动距离超过 100 则立刻设置 scrolled 状态为 true，反之为 false

```jsx
// useState钩子记忆滚动状态
const [scrolled, setScrolled] = useState(false);

// 副作用
useEffect(() => {
	// 对Y轴滚动距离进行检测，
	const handleScroll = () => {
		const scrollTop = window.scrollY;
		if (scrollTop > 100) {
			setScrolled(true);
		} else {
			setScrolled(false);
		}
	};

	// 全局添加滚动监听器
	window.addEventListener("scroll", handleScroll);

	// 在最后返回时记得注销监听器，否则每次副作用被激活都会注册一个新的监听器
	return () => window.removeEventListener("scroll", handleScroll);
}, []);
```

<br>

检测 scrolled 值，实现向下滚动时导航条背景填充，滚动不超过 100 时背景保持透明

```jsx
<nav
	className={`${
		styles.paddingX
	} w-full flex items-center py-5 fixed top-0 z-20 ${
		scrolled ? "bg-primary" : "bg-transparent"
	}`}

    ...
></nav>
```

<br>

### 移动端适配

```jsx
<div className="sm:hidden flex flex-1 justify-end items-center">
	<img
		src={toggle ? close : menu}
		alt="menu"
		className="w-[28px] h-[28px] object-contain"
		onClick={() => setToggle(!toggle)}
	/>

	<div
		className={`${
			!toggle ? "hidden" : "flex"
		} p-6 black-gradient absolute top-20 right-0 mx-4 my-2 min-w-[140px] z-10 rounded-xl`}
	>
		<ul className="list-none flex justify-end items-start flex-1 flex-col gap-4">
			{navLinks.map((nav) => (
				<li
					key={nav.id}
					className={`font-poppins font-medium cursor-pointer text-[16px] ${
						active === nav.title ? "text-white" : "text-secondary"
					}`}
					onClick={() => {
						setToggle(!toggle);
						setActive(nav.title);
					}}
				>
					<a href={`#${nav.id}`}>{nav.title}</a>
				</li>
			))}
		</ul>
	</div>
</div>
```
